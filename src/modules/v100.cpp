/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v100.h"

v100::v100()
{
    name = "Oscillator";
    caption = "OSCILLATOR";
    type_id = 100;
    panel_width = 294;

    //                                         inlets  _______
    //4,472135955                              knobs   ____   \
    //163 286 410 531                          outlets _   \   \
    // 14    271                                        \   \   \

    add_outlet(35,590); // sine                         0   |   |
    add_outlet(89,590); // triangle                     1   |   |
    add_outlet(146,590); // saw                         2   |   |
    add_outlet(200,590); // ramp                        3   |   |
    add_outlet(252,590); // pulse                       4   |   |

    add_knob(215,113,-5.0,0.0,5.0); //frequency             0   |
    add_selector(70,113,5,2); //range                       1   |

    add_inlet(35,481); //pulse width                        |   0
    add_knob(123,481,0.0,0.0,10.0); //pulse width amount    2   |
    add_knob(233,481,0.0,0.5,1.0,5); //pulse width          3   |

    add_inlet( 35,232); //exponential frequency             |   1
    add_inlet( 89,232); //exponential frequency 2           |   2
    add_inlet(146,232); //exponential frequency 3           |   3
    add_knob (233,232,0.0,0.0,12.0,12.0); //exp freq amount 4   |

    add_inlet( 51,358); //hard sync                         |   4

    add_inlet(146,358); //linear freq                       |   5
    add_knob (233,358,0.0,0.0,10.0,10.0); //lin freq amount 5   |

    c = 0.0;
    freq = 234.736420497;
    last_hs_in = 0.0;
}

v100::~v100()
{
    //dtor
}

void v100::dsp(){

    //check for hard sync
    double hs = inlets[4]->pull_sample();
    if(hs > last_hs_in){
        c = 0.0;
    }
    last_hs_in = hs;

    if(!inlets[1]->connection && !inlets[2]->connection){
        int mode = knobs[1]->get_value(); //range selector
        double base, exprange;
        if(mode == 0){ //low
            exprange = 22.360679775;
            base = 1.118033989;
        }else{
            exprange = 2.114742527;
            switch(mode){
                case 1:
                    base = 52.868563175;
                    break;
                case 2:
                    base = 234.736420497;
                    break;
                case 3:
                    base = 1057.3712635;
                    break;
                case 4:
                    base = 4728.564290372;
                    break;
            }
        }
        double finetune = knobs[0]->get_value()/5.0;
        //std::cout << pow(exprange,finetune) << std::endl;
        freq = base * pow(exprange,finetune);
    }else{
        freq = 1.0;
        double one_V_per_octave;
        if(inlets[1]->connection){
            one_V_per_octave = inlets[1]->pull_sample();
            freq *= 8.17579891564 * exp(0.69314718 * one_V_per_octave);
        }
        if(inlets[2]->connection){
            one_V_per_octave = inlets[2]->pull_sample();
            freq *= 8.17579891564 * exp(0.69314718 * one_V_per_octave);
        }
    }

    if(inlets[3]->connection){
        //exp freq modifier
        double mod = 1.0 + inlets[3]->pull_sample() * knobs[4]->get_value() / (5.0*12.0);
        freq *= mod;
    }

    //linear freq modifier
    if(inlets[5]->connection){
        freq += inlets[5]->pull_sample()*knobs[5]->get_value()/10.0;
    }

    double angular_freq = freq/44100.0;

    double pulse_width = (inlets[0]->connection)?(inlets[0]->pull_sample()*0.2)*knobs[2]->get_value()/10.0:0.0;
    pulse_width += knobs[3]->get_value();

    c += angular_freq;
    if(c > 1.0) c -= 1.0;
    double o = sin(c*2*M_PI);
    outlets[0]->push_sample(5.0*o); //sine
    outlets[1]->push_sample(5.0*((c<0.5        )? -4*c + 1.0:  4*c - 3.0)); //triangle
    outlets[2]->push_sample(5.0*((c<0.5        )? -4*c + 1.0: -4*c + 3.0)); //saw
    outlets[3]->push_sample(5.0*((c<0.5        )?  4*c - 1.0:  4*c - 3.0)); //ramp
    outlets[4]->push_sample(5.0*((c<pulse_width)?        1.0:        -1.0)); //pulse
}


void draw_sine(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(-10,0);
    cr->arc(-5,0,5.0,M_PI,0.0);
    cr->stroke();
    cr->arc(5,0,5.0,0.0,M_PI);
    cr->stroke();
    cr->restore();
}

void draw_triangle(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(-10,0);
    cr->line_to(-5,-5);
    cr->line_to(5,5);
    cr->line_to(10,0);
    cr->stroke();
    cr->restore();
}

void draw_saw(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(-10,5);
    cr->line_to(-10,-5);
    cr->line_to(0,5);
    cr->line_to(0,-5);
    cr->line_to(10,5);
    cr->stroke();
    cr->restore();
}

void draw_ramp(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(-10,5);
    cr->line_to(0,-5);
    cr->line_to(0,5);
    cr->line_to(10,-5);
    cr->line_to(10,5);
    cr->stroke();
    cr->restore();
}

void draw_pulse(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(-10,5);
    cr->line_to(-10,-5);
    cr->line_to(0,-5);
    cr->line_to(0,5);
    cr->line_to(10,5);
    cr->line_to(10,-5);
    cr->stroke();
    cr->restore();
}

void v100::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.2);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);


    draw_text(cr,"RANGE",10.0,70,47);
    draw_text(cr,"FREQUENCY",10.0,215,47);
    draw_text(cr,"LOW",10.0,17,114);
    draw_text(cr,"16'",10.0,36,75);
    draw_text(cr,"8'",10.0,71,65);
    draw_text(cr,"4'",10.0,106,77);
    draw_text(cr,"2'",10.0,119,114);

/* The following would require constant refresing.
    char val[10];
    sprintf(val,"%.03lf",angular_freq);
    draw_text(cr,val,12.0,143,139,1.0,0.0,0.0);
*/

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to(14.0 ,163.0);
    cr->line_to(271.0,163.0);
    cr->stroke();
    cr->move_to(14.0 ,286.0);
    cr->line_to(271.0,286.0);
    cr->stroke();
    cr->move_to(14.0 ,410.0);
    cr->line_to(271.0,410.0);
    cr->stroke();
    cr->move_to(14.0 ,531.0);
    cr->line_to(271.0,531.0);
    cr->stroke();

    draw_text(cr,"EXPONENTIAL FREQUENCY",10.0, 90,170);
    draw_text(cr,"1V/OCTAVE",            10.0, 62,197);
    draw_text(cr,"AMOUNT",               10.0,232,170);
    cr->move_to(146,232);
    cr->line_to(179,232);
    cr->stroke();


    draw_text(cr,"HARD SYNC",10.0, 50,294);
    draw_text(cr,"LINEAR",   10.0,146,294);
    draw_text(cr,"FREQUENCY",10.0,146,304);
    draw_text(cr,"AMOUNT",   10.0,232,294);
    cr->move_to(96.0 ,286.0);
    cr->line_to(96.0,410.0);
    cr->stroke();

    draw_text(cr,"WIDTH",      10.0,233,417);
    draw_text(cr,"AMOUNT",     10.0,123,417);
    draw_text(cr,"PULSE WIDTH",10.0, 35,417 ,0.7);
    cr->move_to(35.0 ,481.0);
    cr->line_to(66.0,481.0);
    cr->stroke();

    draw_text(cr,"SINE",    10.0, 35,555);
    draw_text(cr,"TRIANGLE",10.0, 89,555);
    draw_text(cr,"SAW",     10.0,146,555);
    draw_text(cr,"RAMP",    10.0,200,555);
    draw_text(cr,"PULSE",   10.0,252,555);

    cr->set_line_width(1.5);
    cr->translate(35,542);
    draw_sine(cr);
    cr->translate(54,0);
    draw_triangle(cr);
    cr->translate(57,0);
    draw_saw(cr);
    cr->translate(54,0);
    draw_ramp(cr);
    cr->translate(52,0);
    draw_pulse(cr);

    cr->restore();
}
