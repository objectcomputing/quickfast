#!/usr/bin/env python
"""
Create Change Log Entry for a Mercurial repository.
"""
import sys
import os
import traceback
import subprocess
import optparse
import time

def hg_command(options, *args):
    """Run a hg command in path and return the result.
       returns a tuple of strings, stdout and stderr
      Throws on error.
    """    
    arglist = ["hg", "--encoding", "UTF-8", "--noninteractive"] + list(args)
    cmd = " ".join(arglist)
    if options.verbose:
      print "Running: ", cmd
      
    proc = subprocess.Popen(arglist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # proc.communicate returns a tuple with stdout and stderr as utf-8 strings.
    # convert this to a list of decoded to python native strings
    out, err = [utf8string.decode("utf-8") for utf8string in  proc.communicate()]
    
    if proc.returncode:
        raise Exception("Error {0} running {1}:\n".format(proc.returncode, cmd),
          err,out,)
    return (out, err)


def runEditor(options, fileName):
  if options.editor != "none":
    # start the editor.  Do not wait for the results
    subprocess.Popen([options.editor, fileName])


def displaySet(options, title, paths):
  """Display the paths in the set
     options.verbose and options.quiet control the amount of data displayed
     title names the set of paths
  """
  if(not options.quiet):
    if len(paths) > 0:
      print title,"files:",len(paths)
      if(options.verbose):
        for path in paths:
          print '  ',path
    else:
      if(options.verbose):
        print title,"files:",len(paths)


def writeToChangeLog(file, paths, changeType):
  if len(paths) > 0:
    for path in paths:
      file.write("        * " + path + ":\n")
    file.write(  "        " + changeType + "\n")

def genChangeLogTag(options):
  nowstr = time.strftime("%a %b %d %H:%M:%S UTC %Y", time.gmtime())
  username = options.user
  replyto = options.email
  changeLogTag = nowstr + " " + username+ " <" + replyto + ">"
  return changeLogTag


def run(options, args):
  changeLogFileName = "ChangeLog"
  if(len(args) > 0):
    changeLogFileName = args[0]
    if(len(args) > 1):
      print "Too many arguments: ", args[1:]
      return-1
  if not os.path.exists(changeLogFileName):
    print "Change log,", changeLogFileName, "must be an existing file.  Are you in the right directory. "
    return -1
      
  statusText, errorText =  hg_command(options, "status", "-A", "-C")

  # from hg help status:
  #   M = modified
  #   A = added
  #     = origin of the previous file listed as A (added)
  #   R = removed
  #   C = clean
  #   ! = missing (deleted by non-hg command, but still tracked)
  #   ? = not tracked
  #   I = ignored 
  modified = []
  added = []
  removed = []
  missing = []
  clean = []
  unknown = []
  ignored = []
  renamed = []

  for line in statusText.splitlines():
    (sts,space,path) = line.rstrip().partition(' ')
    #print '['+sts+']:'+path
    if sts == 'M':
      modified.append(path)
    elif sts == 'A':
      added.append(path)
    elif sts == '':
      path = path.strip()
      old = added[-1]
      added = added[:-1]
      renamed.append((path, old))
    elif sts == 'R':
      removed.append(path)
    elif sts == 'C':
      clean.append(path)
    elif sts == '!':
      missing.append(path)
    elif sts == '?':
      unknown.append(path)
    elif sts == 'I':
      ignored.append(path)
    else:
      print "Unknown status: '{0}' {1}".format(sts, path)
      
  for rename in renamed:
    removed.remove(rename[0])

  displaySet(options, "Modified", modified)
  displaySet(options, "Added", added)
  displaySet(options, "Removed", removed)
  displaySet(options, "Missing", missing)
  displaySet(options, "Unknown", unknown)
  displaySet(options, "Ignored", ignored)
  displaySet(options, "Clean", clean)
  displaySet(options, "Renamed", renamed)

  if len(modified) + len(added) + len(removed) + len(renamed) > 0 :
    changeLogTag = genChangeLogTag(options)
    tempChangeLogFileName = "..." + changeLogFileName
    if(not options.quiet):
      print "Change Log Tag: ",changeLogTag
    
    ok = False
    with open(tempChangeLogFileName, "w") as changeFile:
      changeFile.write(changeLogTag + '\n');
      writeToChangeLog(changeFile, modified, "Modified")
      writeToChangeLog(changeFile, added, "Added")
      writeToChangeLog(changeFile, removed,"Removed")
      for rename in renamed:
        writeToChangeLog(changeFile, rename, "Renamed")
      changeFile.write("\n")
      with open(changeLogFileName, "r") as oldChangeLog:
        for line in oldChangeLog:
          changeFile.write(line)
        ok = True  
      
    if ok:  
      backupChangeLogFileName = changeLogFileName + ".bak"  
      if os.path.exists(backupChangeLogFileName):
        os.remove(backupChangeLogFileName)
      
      os.rename(changeLogFileName, backupChangeLogFileName)
      os.rename(tempChangeLogFileName, changeLogFileName)
      runEditor(options, changeLogFileName)
      return 0
    return -1  

def defineCommandLineOptions(parser):  
    try:
      userid = os.environ["USER"]
    except:
      try:
        userid = os.environ["USERNAME"]
      except:
        print "Who are you?  Tried USER and USERNAME"
        return -1

    try:
      user = os.environ["CL_USERNAME"]
    except:
      user = userid

    try:
      email = os.environ["REPLYTO"];
    except:
      email = userid + "@ociweb.com"
  
    editor = "none"
    if os.environ.has_key("EDITOR"):
      editor = os.environ["EDITOR"]
    
    parser.add_option("-u", "--user", default=user, help="User name (default: %default)")
    parser.add_option("-m", "--email", default=email, help="Email address (default: %default)")
    parser.add_option("-e", "--editor", default=editor, help="Edit file after creating it using %default.  -e \"none\" disables editing.")
    parser.add_option("-v", "--verbose", action="store_true", default=False, help="Print noise to standard output.")
    parser.add_option("-q", "--quiet", action="store_true", default=False, help="Suppress extra output.")
    return 0
 
def main (argv = None):
  try:
    if(argv == None):
      argv = sys.argv
    parser = optparse.OptionParser(usage = "usage: %prog [options] [changeLogname]\n       Default changeLogname is ChangeLog")
    if defineCommandLineOptions(parser) < 0:
      return -1
    (options, args) = parser.parse_args(argv[1:]) 
    if(options.verbose):
      print "verbose: ", options.verbose
      print "user: ", options.user
      print "email: ", options.email
      print "editor: ", options.editor
      print "positional: ", args
      
    return run(options, args)
  except Exception as ex:
    print "caught exception"
    traceback.print_exc(ex)
    return -1

if __name__ == "__main__":
    sys.exit(main())
