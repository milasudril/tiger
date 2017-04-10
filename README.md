TIGER
=====
Tiger (Texture/Image GenEatoR) loads a set of images and applies a filter multiple times. The result of the filter, can be descriped by the equation

$$
    \vec{y_{k+1}}(x,y) = \vec{f}(x,y,\vec{y_k}(x,y),\vec{g}(x,y),\vec{p})
$$



Command line options
--------------------

This is a summary of all command line options. Values inside square brackets are optional. For details on how to type values, see *Common types* below.

--help[=string]
    Prints usage information to either `stdout`, or to the chosen file.

--example[=string]
    Generates source code for an example filter. The output is printed to either `stdout`, or to the chosen file.

--filter=string
    Specifies the filter file. The filter is either a shared object, or C++ source file. A template filter can be extracted with the `--example` option. When a C++ source file is specified, it will be compiled into a shared object stored. The location of that object is controlled by the option `--objdir`.

--objdir=string
    Specifies where to store any copiled binary, relative to the working directory.

--params[=Parameter,...]
    Sets filter parameters. This is the content of $\vec{p}$ in the equation above.

--params-list[=string]
    Lists all parameters availible for *filter*

--channels-list[=string]
    Lists all channel names used by *filter

--source=Channel mapping,...
    Selects static source images. These images determines $\vec{g}$ in the equation above.

--init=Channel mapping,...
    Selects initial images. These images determines $\vec{y_{0}}$ in the equation above.

--dest=Channel mapping,...
    Selects output images

--iterations=unsigned int
    Selects the number of iterations to apply the filter


Common types
------------

Channel mapping := [Channel:Filename]
    A Channel mapping maps a channel to a given file. Use --channels-list to see a list of availible channels.

Parameter := [Name:Value]
    Parameters control the algorithm used by *filter*. Use --params-list to see a list of parameters availible.

string
    An UTF-8 encoded string

unsigned int
    An integer in the range [0, 4294967295]
