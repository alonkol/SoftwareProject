#include <cstdlib>
#include <cstdbool>
#include <cstdio>
#include <iostream>
extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
}
using namespace std;

int main(int argc,char** argv)
{

    SP_CONFIG_MSG msg;

    SPConfig s = spConfigCreate(argv[1],&msg);
    /** testing for config **/
    /*
    char trys[1024];
    spConfigPrint(s);

    spConfigGetPCAPath(trys,s);
    cout << trys <<endl;
    cout << spConfigIsExtractionMode(s,&msg) << endl;
    cout << spConfigMinimalGui(s,&msg) << endl;
    cout << spConfigGetNumOfImages(s,&msg) << endl;
    cout << spConfigGetNumOfFeatures(s,&msg) <<endl;
    cout << spConfigGetPCADim(s,&msg) <<endl;
    spConfigGetImagePath(trys,s,7);
    cout << trys <<endl;

    spConfigDestroy(s);
    */

    return 0;
}
