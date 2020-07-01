module.exports = (config, env) => {
  config.optimization.runtimeChunk = false;
  config.optimization.splitChunks = {
    cacheGroups: {
      default: false,
    },
  };
  config.output.filename = "[name].[contenthash:8].js";
  const cssPlugin = config.plugins.find(
    (plugin) =>
      plugin.options &&
      plugin.options.filename &&
      plugin.options.filename.indexOf("static/css") === 0
  );
  cssPlugin.options.filename = "[name].[contenthash:8].css";
  return config;
};
