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
    BallController(BallModel* model,BallView* view);
    BallModel* getModel();

    BallModel* ballModel;
    BallView* ballView;

    BallView * getView();
};


#endif //UNTITLED1_BALLCONTROLLER_H
