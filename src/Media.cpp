#include "Media.h"

Media::Media() {

}

void Media::setName(string name) {}

string Media::getName() const {}

void Media::add(Media *m) {
    throw string("Cannot add media!");
}

void Media::removeMedia(Media *m) {}

Media::~Media() {
    //dtor
}
