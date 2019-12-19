# Shell-Remastered
A modified version of Linux shell with basic implementation and some new functionalities

## Utilities Added
* [Copying and Pasting Multiple Files](https://github.com/Hardik500/Shell-Remastered#copying-and-pasting-multiple-files)
* [Simple Classification of Files](https://github.com/Hardik500/Shell-Remastered#simple-classification-of-files)
* [Automation of File Trasfer Protocol](https://github.com/Hardik500/Shell-Remastered#automation-of-file-trasfer-protocol)
* [Listing of files in a tree structure](https://github.com/Hardik500/Shell-Remastered#listing-of-files-in-a-tree-structure)

## Usage
******
### Copying and Pasting Multiple files

#### Command
`copy [file-name]` & `paste`
    
#### Description
    
With the `copy` command just enter the name of the files which you want to copy.

It can be a single file or a group of files.

Then go to a the directory on which you want to paste and enter the `paste` command.

******

### Simple Classification of files
        
#### Command
`classify [file-name]`

#### Description

With the `classify` command just enter the name of the files which you want to classify.
If the file content matches to that of the files (academic.txt, automobile.txt, finance.txt), then it will go to its corresponding folder automatically after running the command.

If the weights are equal then it will display "file cannot be classified".

eg. Try runing : `classify demo_fc.txt`

******

### Automation of File Trasfer Protocol

#### Command
`autoftp`

#### Description

Just run the `autoftp` command and it will get the file for which the name you have set in the bash script i.e. `f.sh`.

>**Warning**: Before running this command modify the bash script according to your preference.

******

### Listing of files in a tree structure

#### Command
`list [level]`

#### Description

With the `list` command just enter the level to which you want to display the tree structure of the folder.
