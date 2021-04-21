# cJRPC2

Ultralightweight C99 JSON-RPC 2.0 library based on cJSON by Dave Gamble.

## Table of contents
* [License](#license)
* [Usage](#usage)
  * [Building](#building)
  * [Example](#example)
* [Contribution](#contribution)

## License

MIT License

>  Copyright (c) 2020-2021 Richard Leitner
>
>  Permission is hereby granted, free of charge, to any person obtaining a copy
>  of this software and associated documentation files (the "Software"), to deal
>  in the Software without restriction, including without limitation the rights
>  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>  copies of the Software, and to permit persons to whom the Software is
>  furnished to do so, subject to the following conditions:
>
>  The above copyright notice and this permission notice shall be included in
>  all copies or substantial portions of the Software.
>
>  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
>  THE SOFTWARE.

## Usage

cJRPC2 aims to be an ultralightweight ANSI C 89 implementation of the
JSON-RPC 2.0 specification at https://www.jsonrpc.org/specification.

It is based on the great cJSON library from Dave Gamble et.al which can
be found at https://github.com/DaveGamble/cJSON.

### Building

The following sections show you how to build and include cJRPC2 in your project.

#### meson subproject

First clone the repository to subprojects/cJRPC2 in your project directory.
Then include the following subproject and dependency definition in your meson.build:
```
cjrpc2_proj = subproject('cJRPC2')
cjrpc2_dep = cjrpc2_proj.get_variable('cjrpc2_dep')
```

Finally add the dependency to your executable:
```
executable('my_exe',
  'main.c',
  dependencies cjrpc2_dep)
```

#### copy source

Alternatively to integrate cJRPC2 as a meson subproject you may just copy the
sources to your project and include them in your build.

### Examples

Please take a look at the examples/ folder.

## Contribution

To report problems or request features please use GitHubs Issue-Tracker feature at https://github.com/g0hl1n/cJRPC2/issues.

To contribute source code to cJRPC2 please use GitHubs Pull-Request feature at https://github.com/g0hl1n/cJRPC2/pulls.
