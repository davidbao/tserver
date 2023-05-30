//
// Created by baowei on 2022/12/16.
//

#include "microservice/SummerStarter.h"
#include "AppStarter.h"

using namespace Microservice;

int main(int argc, const char * argv[]) {
    SummerStarter starter(argc, argv);

    return starter.runLoop<AppStarter>();
}