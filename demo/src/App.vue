<script setup lang="ts">
import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { DatBoolean, DatButton, DatColor, DatFolder, DatGui, DatNumber, DatSelect, DatString } from '@cyrilf/vue-dat-gui';
import { computed, onMounted, reactive, ref, watch, watchEffect } from 'vue';
import type { Body, Model } from 'smplx';
import loadSMPLX from 'smplx';
import '@cyrilf/vue-dat-gui/dist/style.css';

const canvas = ref<HTMLCanvasElement>();

const textureLoader = new THREE.TextureLoader();
const scene = new THREE.Scene();
const geometry = new THREE.BufferGeometry();

let body: Body | undefined;
let model: Model | undefined;

const background = ref('#050505');
watchEffect(() => {
  scene.background = new THREE.Color(background.value);
});

const colorMaterial = new THREE.MeshStandardMaterial({
  color: '#ff00ff',
  flatShading: true,
  side: THREE.DoubleSide,
});

const skinColor = ref('#f0f0f0');
watchEffect(() => {
  colorMaterial.color = new THREE.Color(skinColor.value);
});

const shape1 = ref(0);
const shape2 = ref(0);
const shape3 = ref(0);
const shape4 = ref(0);
const shape5 = ref(0);
const shape6 = ref(0);
const shape7 = ref(0);
const shape8 = ref(0);
const shape9 = ref(0);
const shape10 = ref(0);

watchEffect(() => {
  const shape = [
    shape1.value,
    shape2.value,
    shape3.value,
    shape4.value,
    shape5.value,
    shape6.value,
    shape7.value,
    shape8.value,
    shape9.value,
    shape10.value,
  ];
  if (model && body) {
    const rawShape = body.shape;
    for (let i = 0; i < shape.length; i++) {
      rawShape.set(i, 0, shape[i]);
    }
    body.shape = rawShape;
    body.shape.print('shape');
    // body.update()
    body.update(false, true);

    geometry.attributes.position.needsUpdate = true;
  }
});

const materials = [
  {
    name: 'color',
    value: 'color',
  },
  {
    name: 'male texture',
    value: 'male',
  },
  {
    name: 'female texture',
    value: 'female',
  },
];

const material = ref('color');

const textureMaterial = new THREE.MeshBasicMaterial({ transparent: true });

watchEffect(async () => {
  // textureMaterial.map = await textureLoader.loadAsync(`data/textures/${}.png`)

});

onMounted(async () => {
  const { Gender, Model, Body, FS } = await loadSMPLX();
  const loadFileTo = async (sourcePath: string, targetPath: string) => {
    const data = await (await fetch(sourcePath)).arrayBuffer();
    FS.createDataFile('/', targetPath, new Uint8Array(data), true, true, true);
  };
  const path = 'name';
  const uvPath = 'uv.txt';
  // const outputPath = '/output.obj';
  await loadFileTo('data/models/smpl/uv.txt', uvPath);
  await loadFileTo('data/models/smpl/SMPL_MALE.npz', path);

  model = new Model(path, uvPath, Gender.female);
  body = new Body(model, true);
  // body.setRandom();
  console.log(body.pose.rows(), body.pose.cols());
  console.log(body.shape.rows(), body.shape.cols());
  body.pose.set(9, 0, 0.125);
  // body.shape.set(1, 0, 0.5);
  body.pose.print('pose');
  body.shape.print('shape');

  body.update(true, true);
  const width = canvas.value!.clientWidth;
  const height = canvas.value!.clientHeight;

  const camera = new THREE.PerspectiveCamera(75, width / height, 0.1, 1000);

  camera.position.z = 5;
  const renderer = new THREE.WebGLRenderer({ canvas: canvas.value });
  renderer.setSize(width, height);

  const ambientLight = new THREE.AmbientLight(0x222222);
  scene.add(ambientLight);
  const light1 = new THREE.DirectionalLight(0xFFFFFF, 0.5);
  light1.position.set(1, -1, 1);
  scene.add(light1);
  const light2 = new THREE.DirectionalLight(0xFFFFFF, 1);
  light2.position.set(0, 1, 0);

  const controls = new OrbitControls(camera, renderer.domElement);
  controls.update();

  const posAttr = new THREE.BufferAttribute(body.verts, 3);
  posAttr.setUsage(THREE.DynamicDrawUsage);
  geometry.setAttribute('position', posAttr);

  const uvAttr = new THREE.BufferAttribute(model.uv, 2);
  posAttr.setUsage(THREE.DynamicDrawUsage);
  geometry.setAttribute('uv', uvAttr);

  geometry.setIndex(Array.from(model.faces));

  const mesh = new THREE.Mesh(geometry, colorMaterial);
  scene.add(mesh);

  const box = new THREE.BoxGeometry(1, 1, 1);
  const cube = new THREE.Mesh(box, colorMaterial);
  // scene.add(cube);

  camera.position.z = 5;

  function animate() {
    cube.rotation.x += 0.01;
    cube.rotation.y += 0.01;
    controls.update();
    renderer.render(scene, camera);
  }
  renderer.setAnimationLoop(animate);
});

function random() {
  if (body) {
    body.setRandom();
    body.update(false, true);

    geometry.attributes.position.needsUpdate = true;
  }
}
</script>

<template>
  <div class="root">
    <canvas ref="canvas" />
    <DatGui
      close-text="Close controls"
      open-text="Open controls"
      close-position="bottom"
    >
      <DatColor v-model="background" label="Background" />
      <DatSelect v-model="material" :items="materials" label="Material" />
      <DatButton label="Random params" @click="random" />
      <DatFolder v-if="material === 'color'" label="Color Material">
        <DatColor v-model="skinColor" label="Skin color" />
      </DatFolder>
      <DatFolder label="Shape">
        <DatNumber v-model="shape1" :min="-1" :max="1" :show-slider="true" label="Shape 1" />
        <DatNumber v-model="shape2" :min="-1" :max="1" :show-slider="true" label="Shape 2" />
        <DatNumber v-model="shape3" :min="-1" :max="1" :show-slider="true" label="Shape 3" />
        <DatNumber v-model="shape4" :min="-1" :max="1" :show-slider="true" label="Shape 4" />
        <DatNumber v-model="shape5" :min="-1" :max="1" :show-slider="true" label="Shape 5" />
        <DatNumber v-model="shape6" :min="-1" :max="1" :show-slider="true" label="Shape 6" />
        <DatNumber v-model="shape7" :min="-1" :max="1" :show-slider="true" label="Shape 7" />
        <DatNumber v-model="shape8" :min="-1" :max="1" :show-slider="true" label="Shape 8" />
        <DatNumber v-model="shape9" :min="-1" :max="1" :show-slider="true" label="Shape 9" />
        <DatNumber v-model="shape10" :min="-1" :max="1" :show-slider="true" label="Shape 10" />
      </DatFolder>
    </DatGui>
  </div>
</template>

<style scoped>
.root {
  width: 100%;
  height: 100%;
}

canvas {
  width: 100%;
  height: 100%;
}
</style>
