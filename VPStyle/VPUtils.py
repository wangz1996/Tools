import os
from ROOT import gROOT
gROOT.SetMacroPath(os.pathsep.join([gROOT.GetMacroPath(), os.path.dirname(os.path.realpath(__file__))]))
gROOT.LoadMacro("VPUtils.C")
