\page install Installation guide

# Installation of plugin

## 1. Download and Extract the Plugin

1. Download the ZIP file containing the plugin.
    - From Source [Repository](https://github.com/GrzegorzKrug/UE5_YasiuMath)
    - From FAB [Marketplace](https://www.fab.com/listings/2918e6a2-a380-48a5-90e8-d6c98dfde2a9)

2. Locate your Unreal Engine project folder on your computer.
3. Inside your project folder, create a new folder named **Plugins** if it doesn’t already exist.
    - Example path: `YourProject/Plugins/`
4. Extract (unzip) the downloaded plugin ZIP **into the Plugins folder**.

Your folder structure should look like this:

```
YourProject
├── Content
├── Source
└── Plugins
	└── YasiuMath (put plugin files into this folder)
```

---

## 2A. C++ Dependency

Add dependency to your project in build file: **ProjectName.Build.cs**

```cs
   PrivateDependencyModuleNames.AddRange(new string[] { "YasiuMath" });
```

---

## 2B. Enable the Plugin in Unreal Engine

1. Open your Unreal Engine project.
2. In the top menu, select **Edit → Plugins**.
3. Use the search bar to find **Yasiu Math**.
   1. Should be enabled. If not then check the box to enable the plugin.
4. When Unreal Engine asks you to restart, click **Restart Now**.

---

## 3. Finished!

After the editor restarts, the Plugin is installed and ready to use.

