require "osx/cocoa"
	
include OSX
OSX.require_framework 'ScriptingBridge'
require 'yaml'
finder=OSX::SBApplication.applicationWithBundleIdentifier_("com.apple.Finder")
finder.running?
finder.quit
finder=OSX::SBApplication.applicationWithBundleIdentifier_("com.apple.SystemPreferences")
finder.activate
finder.currentPane
finder.panes[0].properties.each {|x| puts x}
finder.panes.each {|x| finder.currentPane=x if x.properties["id"]=="com.apple.preference.keyboard"}

