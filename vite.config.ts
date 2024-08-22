/// <reference types="vitest" />
import { defineConfig } from 'vite';

export default defineConfig({
  plugins: [],
  test: {
    include: [
      '**/*.{test,spec}.?(c|m)[jt]s?(x)',
      'tests/**/*.ts?(x)',
    ],
  },
});
