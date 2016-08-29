#include <cstdlib>
#include <cstdbool>
#include <cstdio>
#include <iostream>
extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
}
using namespace std;

int main()
{

    SP_CONFIG_MSG msg;
    char trys[1024];
    SPConfig s = spConfigCreate("./config.fig",&msg);
    spConfigPrint(s);
    spConfigGetPCAPath(trys,s);
    cout << trys <<endl;

    cout << spConfigIsExtractionMode(s,&msg) << endl;
    cout << spConfigMinimalGui(s,&msg) << endl;
    cout << spConfigGetNumOfImages(s,&msg) << endl;
    cout << spConfigGetNumOfFeatures(s,&msg) <<endl;
    cout << spConfigGetPCADim(s,&msg) <<endl;
    //spConfigGetImagePath(trys,s,7);  //KORESSSSS
    cout << trys <<endl;
    spConfigGetPCAPath(trys,s);
    cout << trys <<endl;
    spConfigDestroy(s);

    return 0;
}
