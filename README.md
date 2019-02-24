Windows master build: [![Windows build status](https://ci.appveyor.com/api/projects/status/jptep5c7wytewcx2/branch/master?svg=true)](https://ci.appveyor.com/project/lexcelon/batlab-toolkit-gui/branch/master)

**Note:** This software is in development/pre-release.

# Batlab Toolkit GUI

The Batlab Toolkit GUI is a graphical program for controlling Batlabs, scheduling and running tests on battery cells, and configuring battery packs.

## Installation

To install the latest release, visit the [Releases](https://github.com/Lexcelon/batlab-toolkit-gui/releases) page and download the most recent installer for your platform. You will only have to run this installer once.

To upgrade your installation with any changes released since then, you can go to "Help" -> "Check for Updates" inside the Batlab Toolkit GUI program.

## Usage

See the [User Guide](#user-guide) section.

## Contributing

When contributing to this repository, please first discuss the change you wish to make via issue, email, or any other method with the owners of this repository.

### Git branching model

We follow the development model described [here](http://nvie.com/posts/a-successful-git-branching-model/). Anything in the ``master`` branch is considered production. Most work happens in the ``develop`` branch or in a feature branch that is merged into ``develop`` before being merged into ``master``.

### Deployment

Batlab Toolkit GUI is automatically deployed with each tagged commit to ``master``.

To publish a new version, the workflow might look like this. First, make sure you are on the develop branch and then commit your changes:

    $ git status     # Make sure you are on the develop branch
    $ git commit -am "some changes to the develop branch"

Then push them to the remote repository to initiate a build:

    $ git push origin develop

Then you must wait a couple of minutes to make sure the build passes. If the build fails, you will not be able to merge the commit into ``master``. Once the build passes, you can merge into master, create a tagged release and push. When merging, use ``--no-ff`` to preserve the commit and branching history:

    $ git checkout master
    $ git merge --no-ff develop
    $ git tag 0.100.56
    $ git push origin master
    $ git push --tags

When this is done, a few things will happen. A release will be created on GitHub. Appveyor/Travis will perform a build of the source code on their respective platforms. They will also build the installer for that platform and upload it to the GitHub release. They will then use the Qt Installer Framework tools to generate a repository with the built program binaries and upload it to Amazon S3. This way, when users check for updates, the program can read the contents of the S3 repository and determine if updates are available (and download them if so).

## Development Builds

To install a version of Batlab Toolkit GUI that gets updates from the ``develop`` branch, use the following steps. This can be installed independently, alongside a release build. Note that builds from ``develop`` are considered unstable.

First, you may wish to rename the desktop shortcut if you have an existing installation, as this ``develop`` installation will overwrite an existing shortcut of the same name. For example, rename ``Batlab Toolkit GUI`` to ``Batlab Toolkit GUI - Release`` on the desktop.

Just like a release build, visit the [Releases](https://github.com/Lexcelon/batlab-toolkit-gui/releases) page and download the most recent installer.

Run the installer, and on the first page click on "Settings" and then the "Repositories" tab. Click on "User defined repositories" -> "Add" and enter the following URL: ``https://lexcelon-s3-1.s3.amazonaws.com/repository/batlab-toolkit-gui/develop/%platform%`` replacing ``%platform%`` based on your machine (currently only ``windows`` is an option). Uncheck the Updates Repository under "Default repositories". Click "OK" -> "Next".

When prompted to specify an installation directory, give it a different name such as ``C:\Program Files (x86)\Lexcelon\Batlab Toolit GUI_develop``.

Continue the installation as you would for a release build. You may wish to give the new desktop shortcut a name such as ``Batlab Toolkit GUI - Develop``.

# User Guide

The Batlab Toolkit GUI is a tool to control a set of Batlabs and run tests on battery cells.
