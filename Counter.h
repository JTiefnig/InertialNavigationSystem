#include "mbed.h"


class Counter
{
    int state;
    
    Timer t;
    
    bool rt1, rt2;
    int c;
    
    int ti1;
    int ti2;
    
public:
    Counter()
    {
        state=0;
        c=0;
        rt1 = false;
        rt2 = false;
        ti1=0;
        ti2=0;
    }
    
    void evaluate()
    {
        
        if(ti1 < ti2)
        {
            c++;   
        }else
        {
            c--;    
        }
        ti1=0;
        ti2=0;
    }
        
    
    void Flag1()
    {
        
        if(rt2)
        {
            rt2=false;
            t.stop();
            ti2 = t.read_us();        
        }
        
        if(!rt1)
        {
            t.reset();
            rt1=true;
            t.start();
        }
        
        if(ti1 && ti2)
        {
            evaluate();
        }
        
        
    }
    
    void Flag2()
    {
         if(rt1)
        {
            rt1=false;
            t.stop();
            ti1 = t.read_us();        
        }
        
        if(!rt2)
        {
            t.reset();
            rt2=true;
            t.start();
        }
        
        if(ti1 && ti2)
        {
            evaluate();
        }
        
    }
        
    
    int getCount()
    {
        return c;    
    }
    
    int getState()
    {
        state = rt1 + 3*rt2;
        return state;    
    }
    
    
};