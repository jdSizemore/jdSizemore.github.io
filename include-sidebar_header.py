# Use to include code from one file in another.
# Actually copies the text between a start and end tag in one file,
# then pastes that between the same two tags in all other files with
# a particular ending. Achieves this by writing over the files to be
# pasted onto. Does not write over the file from which content is copied,
# or files which do not have the appropriate tags.
#
# Written by Joseph Sizemore 12/29/2020
# Modified to allow multiple sets of tags 5/7/2022

import os
tags = [['<!-- Sidebar -->','<!-- /#sidebar-wrapper -->']
       ,['<!-- Header -->','<!-- /#header-wrapper -->']]
include_file = 'index.html'

def ReadContent(path):
  file = open(path)
  path = file.read()
  file.close()
  return path

def WriteContent(path,content):
  file = open(path,'w')
  file.write(content)
  file.close()

def AppendContent(path,content):
  file = open(path,'a')
  file.write(content)
  file.close()

for tag in tags:
  print('Start Tag : ' + tag[0])
  print('End Tag   : ' + tag[1])
  include = ReadContent(include_file)
  start = include.find(tag[0])
  end = include.find(tag[1],start + len(tag[0]))
  if start == -1 or end == -1:
    print('Start or End Tag Not Found in Target File')
    exit()
  end = end + len(tag[1])
  include = include[start:end]
  
  for filename in os.listdir('.'):
    if filename.endswith('.html'):
      print(filename,end='...')
      if filename == include_file:
        print('Target File, Untouched')
        continue
      content = ReadContent(filename)
      start = content.find(tag[0])
      end = content.find(tag[1],start + len(tag[0]))
      if start == -1 or end == -1:
        print('Start or End Tag Not Found')
        continue
      end = end + len(tag[1])
      content = content[:start] + include + content[end:]
      WriteContent(filename,content)
      print('Content Included')
  print()
