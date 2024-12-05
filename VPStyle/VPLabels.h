//
//   @file    VPLabels.h         
//   
//   @author M.Sutton
// 
//   Copyright (C) 2010 VP Collaboration
//
//   $Id: VPLabels.h, v0.0   Thu 25 Mar 2010 10:34:20 CET $


#ifndef __VPLABELS_H
#define __VPLABELS_H

#include "Rtypes.h"

void VPLabel(Double_t x,Double_t y,const char* text=NULL,Color_t color=1); 

void VPLabelOld(Double_t x,Double_t y,bool Preliminary=false,Color_t color=1); 

void VPVersion(const char* version=NULL,Double_t x=0.88,Double_t y=0.975,Color_t color=1); 

#endif // __VPLABELS_H
