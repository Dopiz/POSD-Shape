#ifndef UTBUILDER_H_INCLUDED
#define UTBUILDER_H_INCLUDED

#include <stack>
#include "TextMedia.h"
#include "SimpleMediaBuilder.h"
#include "CompositeMediaBuilder.h"

TEST (buildTest, Director)
{
    stack<MediaBuilder *> mbs;

    Circle c1(0, 0, 5);
    Circle c2(0, 0, 10);
    Circle c3(0, 1, 10);
    Rectangle r1(0, 0, 3, 2);
    Rectangle r2(0, 0, 5, 4);
    Rectangle r3(0, 1, 8, 7);

    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r1);
    mbs.top()->buildSimpleMedia(&c1);

    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r2);
    mbs.top()->buildSimpleMedia(&c2);
    Media *cm = mbs.top()->getMedia();
    mbs.pop();
    mbs.top()->buildCompositeMedia(cm);

    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r3);
    mbs.top()->buildSimpleMedia(&c3);
    cm = mbs.top()->getMedia();
    mbs.pop();
    mbs.top()->buildCompositeMedia(cm);

    DescriptionVisitor dv;
    mbs.top()->getMedia()->accept(dv);
    CHECK(string("combo( r(0, 0, 3, 2) c(0, 0, 5) combo( r(0, 0, 5, 4) c(0, 0, 10) ) combo( r(0, 1, 8, 7) c(0, 1, 10) ) ) ") == dv.getDescription());
}

TEST (buildCircle, SimpleMediaBuilder)
{
    Circle c(0, 0, 5);

    SimpleMediaBuilder mb;
    mb.buildSimpleMedia(&c);

    Media *m = mb.getMedia();
    DescriptionVisitor dv;
    m->accept(dv);
    CHECK(string("c(0, 0, 5) ") == dv.getDescription());
}

TEST (buildHouse, CompositeMediaBuilder)   //  Combo ---- Combo ---- Combo ---- Rectangle (&r1)
{                                          //    |          |          |
    stack<MediaBuilder *> mbs;             //    |          |           ------- Circle (&c)
                                           //    |           ---- Rectangle (&r2)
    Circle c(12, 5, 2);                    //     ---- Triangle (&t)
    Rectangle r1(10, 0, 15, 5);
    Rectangle r2(0, 0, 25, 20);
    Triangle t(0, 20, 16, 32, 25, 20);

    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&t);
    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r2);
    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r1);
    mbs.top()->buildSimpleMedia(&c);
    Media *cm = mbs.top()->getMedia();
    mbs.pop();
    mbs.top()->buildCompositeMedia(cm);
    cm = mbs.top()->getMedia();
    mbs.pop();
    mbs.top()->buildCompositeMedia(cm);

    //  Before remove.
    DescriptionVisitor dv;
    mbs.top()->getMedia()->accept(dv);
    CHECK(string("combo( t(0, 20, 16, 32, 25, 20) combo( r(0, 0, 25, 20) combo( r(10, 0, 15, 5) c(12, 5, 2) ) ) ) ") == dv.getDescription());

    //  Remove media.
    SimpleMediaBuilder smb;
    smb.buildSimpleMedia(&r2);
    mbs.top()->getMedia()->removeMedia(smb.getMedia());

    // After remove.
    DescriptionVisitor dv2;
    mbs.top()->getMedia()->accept(dv2);
    CHECK(string("combo( t(0, 20, 16, 32, 25, 20) combo( combo( r(10, 0, 15, 5) c(12, 5, 2) ) ) ) ") == dv2.getDescription());
}

TEST (buildHouse1, CompositeMediaBuilder)  //  Combo ---- Triangle  (&t)
{                                          //    |
    stack<MediaBuilder *> mbs;             //  Combo ---- Rectangle (&r2)
                                           //    |
    Circle c(12, 5, 2);                    //  Combo ---- Circle    (&c)
    Rectangle r1(10, 0, 15, 5);            //    |
    Rectangle r2(0, 0, 25, 20);            //     ------- Rectangle (&r1)
    Triangle t(0, 20, 16, 32, 25, 20);

    mbs.push(new CompositeMediaBuilder);
    mbs.push(new CompositeMediaBuilder);
    mbs.push(new CompositeMediaBuilder);
    mbs.top()->buildSimpleMedia(&r1);
    mbs.top()->buildSimpleMedia(&c);
    Media *cm = mbs.top()->getMedia();
    mbs.pop();

    mbs.top()->buildCompositeMedia(cm);
    mbs.top()->buildSimpleMedia(&r2);
    cm = mbs.top()->getMedia();
    mbs.pop();

    mbs.top()->buildCompositeMedia(cm);
    mbs.top()->buildSimpleMedia(&t);

    //  Before remove.
    DescriptionVisitor dv;
    mbs.top()->getMedia()->accept(dv);
    CHECK(string("combo( combo( combo( r(10, 0, 15, 5) c(12, 5, 2) ) r(0, 0, 25, 20) ) t(0, 20, 16, 32, 25, 20) ) ") == dv.getDescription());

    //  Remove media.
    SimpleMediaBuilder smb;
    smb.buildSimpleMedia(&r2);
    mbs.top()->getMedia()->removeMedia(smb.getMedia());

    // After remove.
    DescriptionVisitor dv2;
    mbs.top()->getMedia()->accept(dv2);
    CHECK(string("combo( combo( combo( r(10, 0, 15, 5) c(12, 5, 2) ) ) t(0, 20, 16, 32, 25, 20) ) ") == dv2.getDescription());
}

TEST (textAndBoundingBox, TextMedia)
{
    Rectangle bBox(0, 0, 10, 10);     //  Area: 24     , Perimeter: 20
    Text t(&bBox, "This is text !");
    TextMedia tm(&t);

    DescriptionVisitor dv;
    tm.accept(dv);
    CHECK(string("This is text !") == dv.getDescription());

    //  boundingBox area.
    AreaVisitor av;
    tm.accept(av);
    DOUBLES_EQUAL(100, av.getArea().front(), deviation);

    //  boundingBox perimeter.
    PerimeterVisitor pv;
    tm.accept(pv);
    DOUBLES_EQUAL(40, pv.getPerimeter().front(), deviation);
}

#endif // UTBUILDER_H_INCLUDED
