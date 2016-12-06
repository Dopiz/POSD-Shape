#include "TextUI.h"

TextUI::TextUI() {
    //ctor
}

void TextUI::processCommand() {

    cout << ":- ";

    _content.clear();
    string instruction;

    while(getline(cin, instruction)) {

        cin.clear();

        if(instruction == "")
            processCommand();

        else {

            char *str = new char[instruction.size()];
            strcpy(str, instruction.c_str());

            char *delim = " =.?{}\"";
            char *token = strtok(str, delim);

            while(token != NULL)
            {
                string temp(token);
                _content.push_back(temp);
                token = strtok(NULL, delim);
            }

            string Case = _content[0];

            if(Case == "def")
                this->defineMedia();

            else if(Case == "show")
                this->showMedia();

            else if(Case == "add")
                this->addMedia();

            else if(Case == "delete")
                this->deleteMedia();

            else if(Case == "save")
                this->saveFile();

            else if(Case == "load")
                this->loadFile();

            else
                this->askProperties();
        }
    }
}

void TextUI::defineMedia() {

    string mediaName = _content[1];
    string media = _content[2];

    char *mediaContent = new char[media.size()];
    strcpy(mediaContent, media.c_str());

    switch(mediaContent[0])
    {
        // Shape Media.
        case 'C':
        case 'T':
        case 'R':
        {
            double parameters[6];
            int index = 0;
            bool isNegative = false;
            string p;

            for(int j = 1; mediaContent[j] != ')'; j++) {

                if(mediaContent[j] == '-')
                    isNegative = true;

                else if((mediaContent[j] >= '0' && mediaContent[j] <= '9') || mediaContent[j] == '.') {

                    p += mediaContent[j];

                    if(!((mediaContent[j + 1] >= '0' && mediaContent[j + 1] <= '9') || mediaContent[j + 1] == '.')) {
                        double temp = atof(p.c_str());
                        parameters[index++] = isNegative ? -1 * temp : temp;
                        isNegative = false;
                        p.clear();
                    }
                }
            }

            Shape *shape;

            if(mediaContent[0] == 'C')
                shape = new Circle(parameters[0], parameters[1], parameters[2]);

            else if(mediaContent[0] == 'T')
                shape = new Triangle(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);

            else if(mediaContent[0] == 'R')
                shape = new Rectangle(parameters[0], parameters[1], parameters[2], parameters[3]);

            SimpleMediaBuilder smb;
            smb.buildSimpleMedia(shape);
            smb.getMedia()->setName(mediaName);

            _ms.push_back(smb.getMedia());

            cout << ">> " << media << endl;
            break;
        }

        // combo media.
        case 'c': {

            CompositeMediaBuilder cmb;
            cmb.getMedia()->setName(mediaName);

            string shape = _content[3];

            char *str = new char[shape.size()];
            strcpy(str, shape.c_str());

            char *delim = " ,";
            char *token = strtok(str, delim);

            while(token != NULL)
            {
                string temp(token);
                _content.push_back(temp);

                int number = findMedia(temp);
                cmb.buildCompositeMedia(_ms[number]);

                token = strtok(NULL, delim);
            }

            _ms.push_back(cmb.getMedia());

            break;
        }

        default:
            break;
    }

    processCommand();
}

void TextUI::askProperties() {

    string mediaName = _content[0];
    int number = findMedia(mediaName);

    if(number > -1) {

        string content = _content[1];

        if(content == "area")
            cout << ">> " << _ms[number]->area() << endl;

        else if(content == "perimeter")
            cout << ">> " << _ms[number]->perimeter() << endl;
    }

    else
        cout << ">> Invalid type !" << endl;

    processCommand();
}

void TextUI::addMedia() {

    string shape = _content[1];
    string media = _content[3];

    int sNumber = findMedia(shape);
    int mNumber = findMedia(media);

    _ms[mNumber]->add(_ms[sNumber]);

    cout << _ms[mNumber]->getName() << " = ";
    cout << _ms[mNumber]->getName() << "{ ";
    for(Media *m: _ms[mNumber]->getVector())
        cout << m->getName() << " ";
    cout << "} = " << _ms[mNumber]->description() << endl;

    processCommand();
}

void TextUI::deleteMedia() {

    string shape = _content[1];

    //  delete from combo media.
    if(_content.size() > 2) {
        string media = _content[3];
        int sNumber = findMedia(shape);
        int mNumber = findMedia(media);

        _ms[mNumber]->removeMedia(_ms[sNumber]);
    }

    // delete shape media.
    else
        _ms.erase(_ms.begin() + findMedia(shape));

    processCommand();
}

void TextUI::saveFile() {

    string media = _content[1];
    string fileName = _content[3] + "." + _content[4];

    int number = findMedia(media);

    string content;
    content  = _ms[number]->description() + "\n";
    content += _ms[number]->getName() + "{ ";
    for(Media *m: _ms[number]->getVector())
        content += m->getName() + " ";
    content += "}";

    fstream file;
    file.open(fileName, ios::out | ios::trunc);

    file.write(content.c_str(), content.size());
    cout << ">> " << media << " saved to " << fileName << endl;

    file.close();
    processCommand();
}

void TextUI::loadFile() {

    fstream file;
    string fileName = _content[1] + "." + _content[2];
    MyDocument doc;

    cout << ">> loading " << fileName << " ..." << endl;

    file.open(fileName, ios::in);
    string buffer;
    while(getline(file, buffer))
        cout << buffer << endl;

    file.close();
    processCommand();
}

void TextUI::showMedia() {

    for(Media *m: _ms)
        cout << m->getName() << endl;

    processCommand();
}

int TextUI::findMedia(string name) {

    for(int i = 0; i < _ms.size(); i++) {
        if(name.compare(_ms[i]->getName()) == 0)
            return i;
    }

    return -1;
}

TextUI::~TextUI() {
    //dtor
}