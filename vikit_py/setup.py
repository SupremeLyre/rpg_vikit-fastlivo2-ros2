from setuptools import setup


setup(
    name="vikit_py",
    version="0.0.0",
    packages=["vikit_py"],
    package_dir={"": "src"},
    install_requires=["numpy", "PyYAML"],
)
