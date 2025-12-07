# Installation of plugin

## 1. Download and Extract the Plugin
1. Download the ZIP file containing the plugin.
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
	└── YasiuMath (plugin files here)
```

---

## 2. Enable the Plugin in Unreal Engine
1. Open your Unreal Engine project.
2. In the top menu, select **Edit → Plugins**.
3. Use the search bar to find **Yasiu Math**.
4. Check the box to enable the plugin.
5. When Unreal Engine asks you to restart, click **Restart Now**.

---

## 3. Finished!
After the editor restarts, the Yasiu Math plugin is installed and ready to use.


# C++ Dependency
Add dependency to your project in build file: `ProjectName.Build.cs`
```cs
   PrivateDependencyModuleNames.AddRange(new string[] { "YasiuMath" });
```

