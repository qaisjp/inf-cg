# Computer Graphics 2019 - Assignment 1

Qais Patankar - s1620208 - 2018-10-01

## PBRT

A 15" MacBook Pro was used for this assignment, meaning that we had to install PBRT on macOS.

We initially tried `brew install pbrt` to install pbrt, but Homebrew (the macOS package manager) had an old version of pbrt - _2.0.342_ to be exact.
This is outdated and we need to use [pbrt-v3](https://github.com/mmp/pbrt-v3) instead. We looked into updating the Homebrew formula to install v3 instead,
but since the pbrt-v3 project does not tag releases, the pbrt project is no longer eligible for inclusion in Homebrew.

Therefore, we uninstalled pbrt-v2 (`brew uninstall pbrt`) and then created [a GitHub issue](https://github.com/Homebrew/homebrew-core/issues/44781) to propose the removal of pbrt (v2) from Homebrew.

### Build pbrt

We primarily followed the instructions on the [README.md document](https://github.com/mmp/pbrt-v3/blob/master/README.md).

1. Clone the repository using the recommended `git clone --recursive https://github.com/mmp/pbrt-v3/`
2. Create a `build` directory: `mkdir pbrt-v3/build`
3. Change to the `build` directory: `cd pbrt-v3/build`
4. Generate a Makefile using: `cmake ..`. We don't need to provide any build flags, since the defaults (optimised release build) are perfectly appropriate.

    Copied from the README document:
    > By default, the makefiles that are created that will compile an optimized release build of pbrt. These builds give the highest performance when rendering, but many runtime checks are disabled in these builds and optimized builds are generally difficult to trace in a debugger.

5. Build pbrt-v3 by doing `make -j8`, where `8` is the number of logical cores on your machine.

After pbrt-v3 has been built you should be able to `ls` and see the following output:

```
➜ ls
CMakeCache.txt          CPackSourceConfig.cmake bsdftest                imgtool                 pbrt
CMakeFiles              CTestTestfile.cmake     cmake_install.cmake     libpbrt.a               pbrt_test
CPackConfig.cmake       Makefile                cyhair2pbrt             obj2pbrt                src
```

Note that the `pbrt` and `obj2pbrt`

### Install pbrt

Installing pbrt is as simple as adding your build folder to your `PATH` in `~/.bash_profile` (if you use `bash`).

If you use `sh`, you might need to add it to `~/.profile`. If you use `zsh`, it might be `~/.zprofile`.
If you use `oh-my-zsh` (with `zsh`), you can put it somewhere in `~/.oh-my-zsh/custom`.

1. Assuming you are still in the `build ` directory from the above instructions, type `pwd` and copy the output:

    ```
    ➜ pwd
    /Users/qaisjp/Documents/pbrt-v3/build
    ```

2. Navigate to your home folder and edit your profile` file, and add the following line, editing the path used as appropriate:

    ```
    export PATH="$PATH:/Users/qaisjp/Documents/pbrt-v3/build"
    ```

    When you are doing with using `pbrt-v3` you may remove this line from your PATH.

3. Restart your terminal and you should be able to type `obj2` and press `<tab>` to autocomplete to `obj2pbrt`.

    Test `pbrt --help` and `obj2pbrt --help` to ensure they work.

    **`pbrt --help`**
    ```
    ➜ pbrt --help
    usage: pbrt [<options>] <filename.pbrt...>
    Rendering options:
    --cropwindow <x0,x1,y0,y1> Specify an image crop window.
    --help               Print this help text.
    --nthreads <num>     Use specified number of threads for rendering.
    --outfile <filename> Write the final image to the given filename.
    --quick              Automatically reduce a number of quality settings to
                        render more quickly.
    --quiet              Suppress all text output other than error messages.

    Logging options:
    --logdir <dir>       Specify directory that log files should be written to.
                        Default: system temp directory (e.g. $TMPDIR or /tmp).
    --logtostderr        Print all logging messages to stderr.
    --minloglevel <num>  Log messages at or above this level (0 -> INFO,
                        1 -> WARNING, 2 -> ERROR, 3-> FATAL). Default: 0.
    --v <verbosity>      Set VLOG verbosity.

    Reformatting options:
    --cat                Print a reformatted version of the input file(s) to
                        standard output. Does not render an image.
    --toply              Print a reformatted version of the input file(s) to
                        standard output and convert all triangle meshes to
                        PLY files. Does not render an image.
    ```

    **`obj2pbrt --help`**
    ```
    ➜ obj2pbrt --help
    usage: obj2pbrt [--ptexquads] <OBJ filename> <pbrt filename>
    ```

If the test commands work, then pbrt has been successfully installed.

### Generating your own test image

In this section we test rendering an example scene from the command line.

1. If you are currently in the `pbrt-v3/build` folder, enter the main folder using `cd ..`
2. Enter the `scenes` folder: `cd scenes`
3. Enter `pbrt killeroo-simple.pbrt` to render the scene, this should create a new `killeroo-simple.exr` file.
4. Open this new file using `open killeroo-simple.exr`

You should get a photo like this:

![](killeroo-simple.exr.jpg)

## Approximate model of a scene, necessary to produce photorealistic synthetic objects

Below you can find the scene that will be modelled:

![](../scene-main.jpg)

And the reproduced model:



### Accurate model of the real-world lighting as in the original photograph (10)

### ‘Proxy’ model(s) of real-world object(s) that affect rendered objects (10)

### Textures on real objects that appear on rendered object(s) (20)


## Rendering synthetic object

### Choose a synthetic object model from a 3D model library (.ply files) (5)

### Render of synthetic scene with above parameters (5)


## Compositing / Merging of synthetic objects on the real-world image

### Photometric consistency, i.e. light intensity, direction, color, shadows (10)

### Seamless positional compositing – no mismatched edges (5)

### Shadows of rendered object on real scene (5)

### Shadows of real objects on rendered elements (5)

### Reflections of textured obects on rendered objects (reflective material) (5)

### Multiple rendered objects (5)
