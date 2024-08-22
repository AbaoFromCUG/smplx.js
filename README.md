![Check](https://github.com/AbaoFromCUG/smpl.js/workflows/Check/badge.svg)
[![npm version](https://badge.fury.io/js/smpl.svg)](https://badge.fury.io/js/smplx)

# SMPLX.js

`SMPLX-X` JavaScript/WebAssembly implementation in `smplxcpp` and `emscripten`

## Features

- Type safety through [TypeScript](https://www.typescriptlang.org)

## Usage

### Get started

```typescript

import fs from 'node:fs/promises';
import loader, { Gender } from "smplx";

const loadFileTo = async (sourcePath: string, targetPath: string) => {
  const data = await fs.readFile(sourcePath);
  FS.createDataFile('/', targetPath, data, true, true, true);
};
const path = 'name';
const uvPath = 'uv.txt';
const outputPath = '/output.obj';
await loadFileTo('data/models/smpl/uv.txt', uvPath);
await loadFileTo('data/models/smpl/SMPL_FEMALE.npz', path);

const model = new Model(path, uvPath, Gender.female);
const body = new Body(model, false);
body.setRandom();

body.update(false, true);
body.saveObj(outputPath);
const content = FS.readFile(outputPath) as Uint32Array;
const decoder = new TextDecoder();

const str = decoder.decode(content);
console.log(str)

body.delete();
model.delete();
```

# Contribution

## Build

To be able to build WebAssembly code from C++ using Emscripten, you must first [install and activate the emsdk](https://emscripten.org/docs/getting_started/downloads.html).
To compile the C++ code to WebAssembly, run the following command from the project's root directory.

```shell
pnpm install
```

## Run tests

The following command will build and run the test suite.

```shell
pnpm test
```

For rapid developing, tests can also be started in watch mode, which will automatically run on any code change to the TypeScript or JavaScript sources.

```shell
pnpm start
```

### Fix code style

The following command will run prettier on the TypeScript and clang-format on the C++ source code.

```shell
pnpm run lint:fix
```

# Acknowledges

- [smplxpp](https://github.com/sxyu/smplxpp): super fast SMPL/+H/-X implementation in C++, with CUDA support and a built-in OpenGL renderer
- [modern-wasm-starter](https://github.com/TheLartians/modern-wasm-starter): A starter template to easily create WebAssembly packages for npm using type-safe C++ code with automatic declarations
