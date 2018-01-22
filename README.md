Windows master build: [![Windows build status](https://ci.appveyor.com/api/projects/status/jptep5c7wytewcx2/branch/master?svg=true)](https://ci.appveyor.com/project/lexcelon/batlab-toolkit-gui/branch/master)

**Note:** This software is in development/pre-release.

# Batlab Toolkit GUI

The Batlab Toolkit GUI is a graphical program for controlling Batlabs, scheduling and running tests on battery cells, and configuring battery packs.

## Installation

To install the latest release, visit the [Releases](https://github.com/Lexcelon/batlab-toolkit-gui/releases) page and download the most recent installer for your platform. You will only have to run this installer once.

To upgrade your installation with any changes released since then, you can go to "Help" -> "Check for Updates" inside the Batlab Toolkit GUI program.

## Contributing

When contributing to this repository, please first discuss the change you wish to make via issue, email, or any other method with the owners of this repository.

### Git branching model

We follow the development model described `here <http://nvie.com/posts/a-successful-git-branching-model/>`_. Anything in the ``master`` branch is considered production. Most work happens in the ``develop`` branch or in a feature branch that is merged into ``develop`` before being merged into ``master``.

### Deployment

Batlab Toolkit GUI is automatically deployed with each tagged commit to ``master``.

To publish a new version, the workflow might look like this. First make your changes:

```
$ git commit -am "some changes to the develop branch"
```

Then, update ``setup.py`` and ``docs/source/conf.py`` with the new version number (in this case we will use the example 0.100.56). Make a commit with these changes and push it to GitHub:

.. code-block:: bash

   $ git commit -am "rev version number to 0.100.56"
      $ git push origin develop

Then you must wait a couple of minutes to make sure the build passes on Travis. If the build fails, you will not be able to merge the commit into ``master``. Once the build passes, you can merge into master, create a tagged release and push. When merging, use ``--no-ff`` to preserve the commit and branching history:

.. code-block:: bash

   $ git checkout master
      $ git merge --no-ff develop
         $ git tag v0.100.56
	    $ git push origin master
	       $ git push --tags

Changes should automatically roll out to PyPi, and any documentation included in your code will automatically roll out to Read the Docs.