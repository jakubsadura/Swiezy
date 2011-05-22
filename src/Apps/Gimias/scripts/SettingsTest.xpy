# ScriptVersion=LambdaCore_v1.1.6.1
# DO NOT MODIFY THIS HEADER!
# This file can be non-compatible with different versions of Lambda Core
#

print "Test [ Settings ]"

settings = Core.Kernel.GetApplicationSettings()

print "Current build: ", settings.GetApplicationTitleAndVersion()

print "Reload application Settings"
settings.LoadSettings()

if settings.IsFirstTimeStart():
	print "Application first time start: true"
else:
	print "Application first time start: false"

print "Set first time start: true"
settings.SetFirstTimeStart(1)

if settings.IsFirstTimeStart():
	print "Application first time start: true"
else:
	print "Application first time start: false"
	
print "Save new application Settings"
settings.SaveSettings();

print "Log filename is:", settings.GetLogFilename()
print "Working dir is:", settings.GetWorkingDir()
print "Authors are:", settings.GetAuthorsList()

print "Current profile is:", settings.GetProfileAsString()
