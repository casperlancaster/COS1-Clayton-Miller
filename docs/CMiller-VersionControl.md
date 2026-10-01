# Instructions

Update this document where indicated [look for the brackets!]. Replace text inside the brackets with your own information. For example: Course Name should be the name of this course, and not the generic words "Course Name".

<br>

## COS119

- **Student:** Clayton Miller
- **Date:** September 30, 2026
- **Due Date:** October 4, 2026

This paper addresses some of the topic matter covered in research and activity this week. Be sure to include reference links below to the research and information you used to complete this assignment.

## Topic: Terminal

Professional developers use Terminal daily. It's essential to understand some fundamental commands to use the application.

Update the information below to demonstrate your knowledge on this topic.

**1. Using Terminal, there are essential commands to know.**

List the correct Terminal commands to do the actions listed below. Replace **CMD** with the correct command sequence. You can keep or enhance the brief description.

**The last bullet provides an example**.

-  clear : Clear the Screen
-  pwd : Print the "Working Directory"
-  ls : List files and folders
-  ls -a : List files and folders, including invisible files
-  ls -lah : List all files and folders, in human readable form
-  cd <directory>: Change directory
-  cd / : Change directory, go to root directory
-  cd ~ : Change directory and go to user home directory
-  cd .. : Change directory, go up one folder level
-  cd ../.. : Change directory, go up two folder levels
-  cd ~/Desktop : Change directory to my desktop!

**2. Using Terminal...**

**Folder Drop:** Try typing "cd" followed by a space, and then drag a folder into terminal and press return. Test this out and describe your results below.

 When I typed cd followed by a space and dragged the docs folder into Terminal, Terminal automatically inserted the full path to the folder. After I pressed Enter, the working directory changed to the docs folder. I then used the pwd command to confirm that Terminal was working from /c/Projects/COS1-Clayton-Miller/docs. This showed me that dragging a folder into Terminal is a quick way to enter a directory path without typing the entire path manually. 

## Topic: Version Control & Git

Version control, also known as revision control, records changes to a file or set of files over time so that you can recall specific versions later. In this class, we are learning Git. Update the information below where indicated.

**1. There are three types of version control.**

1. **Local Version Control:** Local version control stores different versions of files on a single computer. It allows a developer to track changes and return to an earlier version, but the history is only stored locally on that computer.

2. **Centralized Version Control:** Centralized version control uses one central server that stores the main repository and version history. Developers connect to the server to access and update files. This makes collaboration easier, but if the central server becomes unavailable, access to the repository may be interrupted.

3. **Distributed Version Control:** Distributed version control gives each developer a complete copy of the repository and its history on their own computer. Developers can make commits locally and later share their changes with a remote repository. Git is an example of a distributed version control system.

**2. Using Terminal, there are also essential Git commands to know.**

List the correct Git commands to do the actions listed below in Terminal. Replace CMD with the correct command and keep or enhance the brief description.

-  git clone <repository-url> : Clone a repository
-  git config --global user.name "Casper Lancaster" : Set-up a global user name
-  git config --global user.email "Casperlancaster94@gmail.com" : Set-up a global email address (to match my GitHub account email)
-  git status : Shows the current state of your directory and staging area
-  git add <file> : Add modified files to the next commit
-  git commit -m "commit message" : Make a commit with a new message
-  git log : Show my commit history
-  git help : Show Git's help screen

**3. Connecting to GitHub using Terminal.**
HTTPS is the the correct way to connect to GitHub in this course. Describe how you connect to GitHub from Terminal using this protocol. What steps do you take?

 To connect to a GitHub repository using HTTPS, I first copy the repository's HTTPS URL from GitHub. In Terminal, I navigate to the folder where I want the repository to be stored. I can then use `git clone` followed by the HTTPS URL to create a local copy of the repository. If I already have a local Git repository, I can connect it to GitHub with `git remote add origin` followed by the HTTPS URL. I can use `git remote -v` to verify that the remote repository is connected correctly. After the connection is established, I can use `git pull` to retrieve changes from GitHub and `git push` to upload my local commits. GitHub may require me to authenticate through my account or browser when using HTTPS. 

**4. Using .gitignore and Why it's Important**  
Most repositories contain a .gitignore file.

- What is the purpose of this file?
  <br>
   A `.gitignore` file tells Git which files and folders should not be tracked or included in commits. This is useful for excluding temporary files, generated files, personal settings, and other files that do not belong in the repository. 

- What is the "**.DS_Store**" file and why would you want to ignore it?
  <br>
   `.DS_Store` is a hidden file created by macOS Finder to store folder display settings and metadata. It is usually ignored because it is specific to one computer and is not needed for the project source code. 

- What other file or folder would you want to add to a .gitignore file and why?
  <br>
   Another file or folder I would add to `.gitignore` is the `.vs/` folder created by Visual Studio. It contains local Visual Studio settings and temporary development files that are specific to my computer and do not need to be shared in the GitHub repository. 

<br>

# Reference Links

Replace the example references below with your own links and recommended resources. It is acceptable to provide multiple links for a single topic and to use material provided to you in this class. You are encouraged to link to your own independent research as well.

 The resources I found most helpful this week were the official Git documentation and GitHub documentation. The Git documentation helped me understand the differences between local, centralized, and distributed version control, while the GitHub documentation helped me understand how repositories are cloned and connected using HTTPS. I also found the command documentation helpful because I was able to use many of the Git and Terminal commands while working on my own project this week. 

**Terminal Commands**  
[GNU Coreutils Manual](https://www.gnu.org/software/coreutils/manual/coreutils.html)

**Three Types of Version Control**  
[Git - About Version Control](https://git-scm.com/book/en/v2/Getting-Started-About-Version-Control.html)

**Git Commands**  
[Git Reference](https://git-scm.com/docs)

**Connecting to GitHub using Terminal**  
[GitHub Docs - Cloning a repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository)

**Using .gitignore and Why it's Important**  
[GitHub Docs - Ignoring Files](https://docs.github.com/en/get-started/git-basics/ignoring-files)
