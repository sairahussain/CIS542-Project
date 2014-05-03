#include "compass.h"

HMC6352 direction(p9, p10);

compass::compass()
{
    //Continuous mode, periodic set/reset, 20Hz measurement rate.
    direction.setOpMode(HMC6352_CONTINUOUS, 1, 20);
}

compass::~compass(){}

int compass::getPos()
{
    wait(0.1);
    return ((int)direction.sample()/10);
}

int compass::gotoPos(int final_pos)
{
        Navigate boebot;
        int init_pos;
        int new_pos;
        int turn_right_dist, turn_left_dist;
        
        init_pos = getPos();
        
        if(final_pos < init_pos)
        {
            turn_right_dist = final_pos + (360 - init_pos);
            turn_left_dist = init_pos - final_pos;
        }
        
        else if (final_pos >= init_pos)
        {
            turn_right_dist = final_pos - init_pos;
            turn_left_dist = (360 - final_pos) + init_pos;
        }
         
        if(turn_left_dist < turn_right_dist)
        {
                boebot.left();
        }
        else 
        {
                boebot.right();
        }
        new_pos = getPos();
                
        while(!(new_pos <= final_pos+2 && new_pos >=final_pos-2))
        {
            new_pos = getPos();
        }   
        boebot.stop(); 
        
        return 0;      
}

