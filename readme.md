# How to Build
After cloning the repo, make sure to run:

```bash
git submodule update --init
```

This will pull the source code from the Raylib repo. At the time of writing 
this readme, the submodule is set to look at Raylib release v3.7.0

## Mac
We will be building the project statically and without xcode.

First ensure that you have xcode tools installed. Update in the app store after if needed.
```bash
xcode-select --install
```

Then run
```bash
sh build.sh
```

## Windows

First ensure that you have the MinGw compiler toolchain installed.
The easiest way to do this is by installing Raylib, then setting
in the PATH environment variable a item pointing to C:\raylib\mingw\bin

Then run
```bash
build.bat
```

If you are running the build script from inside powershell (which happens when you try to run the script from Clion or Visual Studio Code), then make sure to run
```bash
./build.bat
```

### Possible errors on Windows

I found that I had two versions of g++ installed, resulting in 64 bit binaries and 32 bit gdb.
This meant that I was unable to debug the binaries after building, heavily impeding development.

To check for multiple versions run the following.
```bash
where g++
``` 

## Git and JIRA Workflow

Step 1:

Go into JIRA, click on the issue, scroll down, hit "Create branch".
Give the branch a short name and remember that name. Let's call it "branch_name"

On your computer run
```bash
git checkout master
git pull
git checkout -b "branch_name"
```

Write the code for the feature. Make many commits, etc. 

Once you are ready to upload the feature to the remote repository...

Run
```bash
git push
```

Git will complain. Do exactly what is says to do and all will be well. Finally, 
create a pull request in JIRA for your issue.

Once the entire team has given the code the "go-ahead",
you can resolve the pull request and merge the feature into the master branch.


