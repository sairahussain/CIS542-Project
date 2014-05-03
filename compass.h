#include "HMC6352.h"
#include "boebot.h"

//documentation? what is this?
class compass{    
    public: 
        
        compass();
        ~compass();
        int getPos();
        int gotoPos(int final_Pos);

};

