#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class InvalidNoteException {
};

struct Accidental {
    Accidental(const string &display_name, int semitoneDiff) : display_name(display_name), semitoneDiff(semitoneDiff) {}

    string display_name;
    int semitoneDiff; //flat = -1; natural = 0; sharp = 1;
};

enum tones {
    A, B, C, D, E, F, G
};

class Tone {
public:
    explicit Tone(int toneNum) : tone_num(toneNum) {
        if (toneNum >= 0 && toneNum <= 6) {
            tone_num = toneNum;
            display_name = static_cast<char>('A' + toneNum);
        } else {
            tone_num = toneNum - 'A';
            display_name = static_cast<char>(toneNum);
        }

        switch (toneNum) {
            case A:
            case 'A':
                numerical_value = 1;
                break;
            case B:
            case 'B':
                numerical_value = 3;
                break;
            case C:
            case 'C':
                numerical_value = 4;
                break;
            case D:
            case 'D':
                numerical_value = 6;
                break;
            case E:
            case 'E':
                numerical_value = 8;
                break;
            case F:
            case 'F':
                numerical_value = 9;
                break;
            case G:
            case 'G':
                numerical_value = 11;
                break;
            default:
                display_name = 'A';
                numerical_value = '1';
                break;
        }
    }

    Tone operator-(int subtractBy) {
        int newToneNum = tone_num - subtractBy;
        if (newToneNum < 0) {
            newToneNum += 6;
        } else if (newToneNum > 6) {
            newToneNum -= 6;
        }
        return Tone(newToneNum);
    }

    Tone operator+(int addBy) {
        int newToneNum = tone_num + addBy;
        if (newToneNum < 0) {
            newToneNum += 6;
        } else if (newToneNum > 6) {
            newToneNum -= 6;
        }
        return Tone(newToneNum);
    }

    int numericalValue() {
        return numerical_value;
    }

    char displayName() {
        return display_name;
    }

private:
    char display_name = 'a';
    int numerical_value = 0;
    int tone_num = A;
};

Accidental flat{"♭", -1};
Accidental natural{"", 0};
Accidental sharp{"♯", 1};

class Note {
public:
    Note getRelativeKey();

    Note(Tone &tone, int numericalValue, bool isMajor);

    explicit Note(string noteStr);

    int numerical_value();

    string display_name();

private:
    bool is_major = true;
    Tone tone = Tone(A);
    Accidental accidental = natural;

};

Note Note::getRelativeKey() {
    if (is_major) {
        Tone newTone = tone - 2;
        return Note(newTone, numerical_value() - 3, false);
    } else {
        Tone newTone = tone + 2;
        return Note(newTone, numerical_value() + 3, true);
    }
}

int Note::numerical_value() {
    return tone.numericalValue() + accidental.semitoneDiff;
}

string Note::display_name() {
    return is_major ? tone.displayName() + accidental.display_name + "+" : tone.displayName() +
                                                                           accidental.display_name + "-";
}

Note::Note(Tone &tone, const int numericalValue, bool isMajor) {
    this->tone = tone;
    is_major = isMajor;

    int value = tone.numericalValue();
    int accidental_value = numericalValue - value;

    switch (accidental_value) {
        case -1:
            accidental = flat;
            break;
        case 0:
            accidental = natural;
            break;
        case 1:
            accidental = sharp;
            break;
        default:
            cerr << "Invalid Input" << endl;
            throw InvalidNoteException();
    }
}

Note::Note(const string noteStr) {
    if (!noteStr.empty()) {
        tone = Tone(noteStr[0]);
    }
    if (noteStr.size() > 1) {
        switch (noteStr[1]) {
            case '#':
                accidental = sharp;
                break;
            case 'b':
                accidental = flat;
                break;
            case '+':
                accidental = natural;
                is_major = true;
                break;
            case '-':
                accidental = natural;
                is_major = false;
                break;
            default:break;
        }
    }
    if (noteStr.size() > 2) {
        switch (noteStr[2]) {
            case '+':
                is_major = true;
            case '-':
                is_major = false;
            default:break;
        }
    }
}

int main() {
    cout << "input note: ";
    string input;
    cin >> input;
    Note *note = new Note(input);
    cout << "numerical value of '" << note->display_name() << "' is " << note->numerical_value() << endl;
    cout << "it's relative key is " << note->getRelativeKey().display_name() << endl;
    return 0;
}
