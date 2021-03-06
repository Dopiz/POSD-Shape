#ifndef TEXTUI_H
#define TEXTUI_H
#include "TextUIHeader.h"
#include "CommandManager.h"

class TextUI
{
    private:
        vector<string> _content;
        vector<Media *> _ms;
        CommandManager _commandManager;

    protected:
        void instructionAnalysis(string instruction, char *delim);
        void defineMedia();
        void askProperties();
        void addMedia();
        void deleteMedia();
        void deleteFromCompositeMedia();
        void saveFile();
        void loadFile();
        void showMedia();
        int findMedia(string name);

    public:
        TextUI();
        void enterInstruction();
        void processCommand(string instruction);
        virtual ~TextUI();
};
#endif // TEXTUI_H
