//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALLCONTROLLER_H
#define UNTITLED1_BALLCONTROLLER_H


#include "Entity.h"
#include "BallModel.h"
#include "BallView.h"

class BallController {


public:
    BallModel* getModel();

    BallView * getView();

    static void initialize(BallModel *model, BallView *view);


    static BallController *getInstance();

    void kick();

    void update(double d, int i, int i1);

private:
    BallController(BallModel* model, BallView* view);

    BallModel* ballModel;

    BallView* ballView;


};


#endif //UNTITLED1_BALLCONTROLLER_H
