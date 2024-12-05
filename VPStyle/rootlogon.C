#include "VPStyle.C"
void rootlogon()
{
  // Load VP style
  //gROOT->LoadMacro("VPStyle.C"); //No longer works for ROOT6
  SetVPStyle();
}
