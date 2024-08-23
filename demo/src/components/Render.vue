<script setup lang="ts">
import { onMounted, ref } from 'vue';
import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import loadSMPLX from 'smplx';

const canvas = ref<HTMLCanvasElement>();
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
  await loadFileTo('data/models/smpl/SMPL_FEMALE.npz', path);

  const model = new Model(path, uvPath, Gender.female);
  const body = new Body(model, false);
  body.setRandom();
  body.pose.set(15, 0, 0.5);

  body.update(false, true);
  const width = canvas.value!.clientWidth;
  const height = canvas.value!.clientHeight;

  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0x050505);
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

  const geometry = new THREE.BufferGeometry();

  const posAttr = new THREE.BufferAttribute(body.verts, 3);
  posAttr.setUsage(THREE.DynamicDrawUsage);
  geometry.setAttribute('position', posAttr);

  const uvAttr = new THREE.BufferAttribute(model.uv, 2);
  posAttr.setUsage(THREE.DynamicDrawUsage);
  geometry.setAttribute('uv', uvAttr);

  geometry.setIndex(Array.from(model.faces));

  const material = new THREE.MeshStandardMaterial({
    color: '#ff00ff',
    flatShading: true,
    side: THREE.DoubleSide,
  });
  const mesh = new THREE.Mesh(geometry, material);
  scene.add(mesh);

  const box = new THREE.BoxGeometry(1, 1, 1);
  const cube = new THREE.Mesh(box, material);
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
</script>

<template>
  <canvas ref="canvas" />
</template>

<style scoped>
canvas {
  width: 100%;
  height: 100%;
}
</style>
