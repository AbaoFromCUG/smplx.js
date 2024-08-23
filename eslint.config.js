import antfu from '@antfu/eslint-config';

export default antfu({
  stylistic: {
    indent: 2, // 4, or 'tab'
    quotes: 'single', // or 'double'
  },
  typescript: true,
  jsonc: false,
  vue: true,
}, {
  rules: {
    // 'no-console': 'off',
    'style/semi': ['error', 'always'],
  },
});
