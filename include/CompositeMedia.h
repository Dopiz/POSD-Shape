#ifndef COMPOSITEMEDIA_H
#define COMPOSITEMEDIA_H
#include "Media.h"
#include <vector>

class CompositeMedia : public Media
{
    private:
        vector<Media *> _media;
        string _name;

    public:
        CompositeMedia();
        CompositeMedia(vector<Media *> m);
        void setName(string name);
        string getName() const;
        void add(Media *m);
        void removeMedia(Media *m);
        void accept(MediaVisitor &v);
        double area() const;
        double perimeter() const;
        string description() const;
        ~CompositeMedia();
};

#endif // COMPOSITEMEDIA_H
