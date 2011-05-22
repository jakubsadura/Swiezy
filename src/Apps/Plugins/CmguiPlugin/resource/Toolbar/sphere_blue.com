#------------------------------------------------------------------------------------------------------------------------------
# Script to change surface color to blue
# by Xavi Planes. Nov 2010
#------------------------------------------------------------------------------------------------------------------------------
#

gfx create material blue alpha 1
gfx modify g_element ${region} surfaces material blue scene ${scene}
