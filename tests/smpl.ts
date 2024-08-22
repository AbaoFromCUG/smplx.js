import fs from 'node:fs/promises';
import { describe, expect, test } from 'vitest';
import loader, { Gender } from "smplx";

test('load smplx', async () => {
  const { Gender, Model, Body, FS } = await loader();

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
  body.pose.set(15, 0, 0.5);

  body.update(false, true);
  body.saveObj(outputPath);
  const content = FS.readFile(outputPath) as Uint32Array;
  const decoder = new TextDecoder();

  const str = decoder.decode(content);
  expect(str).toBeTruthy();
  expect(str.length).toBe(441161);

  body.delete();
  model.delete();
});
