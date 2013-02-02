/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V400_H
#define V400_H

#include "../Module.h"

/*
 * 2-channel mixer module
 */
class v400 : public Module
{
    public:
        v400();
        virtual ~v400();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V400_H
