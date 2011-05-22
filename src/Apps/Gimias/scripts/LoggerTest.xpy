# ScriptVersion=LambdaCore_v1.1.6.1
# DO NOT MODIFY THIS HEADER!
# This file can be non-compatible with different versions of Lambda Core
#

print "Test [ Logger ]"

logger = Core.Kernel.GetLogManager();

print "Got filename", logger.GetFile()

logger.Clear()
print "Logfile cleared"

filename = "logFile.txt"
print "Logfile set to", filename
logger.SetFile(filename)

filename = logger.GetFile()
print "Got filename", filename

logger.LogMessage("Logger test completed")
print "Message <Logger test completed> added to the logfile", filename

print "\n"

del logger;