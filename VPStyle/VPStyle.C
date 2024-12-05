//
// VP Style, based on a style file from BaBar
//

#include <iostream>

#include "VPStyle.h"

#include "TROOT.h"

void SetVPStyle ()
{
  static TStyle* vpStyle = 0;
  std::cout << "\nApplying VP style settings...\n" << std::endl ;
  if ( vpStyle==0 ) vpStyle = VPStyle();
  gROOT->SetStyle("VP");
  gROOT->ForceStyle();
}

TStyle* VPStyle() 
{
  TStyle *vpStyle = new TStyle("VP","VP style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  vpStyle->SetFrameBorderMode(icol);
  vpStyle->SetFrameFillColor(icol);
  vpStyle->SetCanvasBorderMode(icol);
  vpStyle->SetCanvasColor(icol);
  vpStyle->SetPadBorderMode(icol);
  vpStyle->SetPadColor(icol);
  vpStyle->SetStatColor(icol);
  //vpStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  vpStyle->SetPaperSize(20,26);

  // set margin sizes
  vpStyle->SetPadTopMargin(0.05);
  vpStyle->SetPadRightMargin(0.05);
  vpStyle->SetPadBottomMargin(0.16);
  vpStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  vpStyle->SetTitleXOffset(1.4);
  vpStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  vpStyle->SetTextFont(font);

  vpStyle->SetTextSize(tsize);
  vpStyle->SetLabelFont(font,"x");
  vpStyle->SetTitleFont(font,"x");
  vpStyle->SetLabelFont(font,"y");
  vpStyle->SetTitleFont(font,"y");
  vpStyle->SetLabelFont(font,"z");
  vpStyle->SetTitleFont(font,"z");
  
  vpStyle->SetLabelSize(tsize,"x");
  vpStyle->SetTitleSize(tsize,"x");
  vpStyle->SetLabelSize(tsize,"y");
  vpStyle->SetTitleSize(tsize,"y");
  vpStyle->SetLabelSize(tsize,"z");
  vpStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  vpStyle->SetMarkerStyle(20);
  vpStyle->SetMarkerSize(1.2);
  vpStyle->SetHistLineWidth(2.);
  vpStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars (as recommended in VP figure guidelines)
  //vpStyle->SetErrorX(0.0001); // this prevents the E2 draw option from working, use X0 option instead
  // get rid of error bar caps
  vpStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  vpStyle->SetOptTitle(0);
  //vpStyle->SetOptStat(1111);
  vpStyle->SetOptStat(0);
  //vpStyle->SetOptFit(1111);
  vpStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  vpStyle->SetPadTickX(1);
  vpStyle->SetPadTickY(1);

  return vpStyle;

}

