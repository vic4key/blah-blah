# 💻 SETUP VSCODE FOR ANDROID (TERMUX)

This document provides a comprehensive walkthrough for installing Visual Studio Code (code-server web distribution) on Termux, configuring remote network options, and deploying a permanent patch against Android 14/15 sandbox isolation crashes (`Connection closed` / `Killed`).

---

## 📥 STEP 1: INSTALL VS CODE (CODE-SERVER)

Open Termux on your handset and run the following command to pull the official environment build managed by the Termux community:

```bash
pkg update && pkg install code-server -y
```
*(The system manager will configure Node.js and extract the core VS Code layout binaries in about 1–2 minutes).*

---

## ⚙️ STEP 2: NETWORK CONFIGURATION FOR REMOTE PC ACCESS

By default, the server restricts bindings strictly to localhost (`127.0.0.1`). You must update the profile to route traffic from your desktop PC across Wi-Fi or ADB connections:

1. **Fire up the execution layer for the first time to generate default configurations:**
   ```bash
   code-server
   ```
   *As soon as the console logs begin scrolling, press **`Ctrl` + `C`** to abort the instance.*

2. **Open the newly generated environment file with nano:**
   ```bash
   nano ~/.config/code-server/config.yaml
   ```

3. **Modify the layout parameters exactly as shown below:**
   * Locate the `bind-addr` and `password` blocks and replace them using this structure:
     ```yaml
     # Original file configuration:
     bind-addr: 127.0.0.1:8080
     auth: password
     password: <your-old-password>
     cert: false

     # Modified configuration:
     bind-addr: 0.0.0.0:8080
     password: <your-new-password>
     ```
   * Shifting the bind mask to `0.0.0.0` explicitly allows external network cards to intercept port 8080.
   * Provide your secure string token in `<your-new-password>` to lock down entry from your PC.
   * Save and drop out of the editor: press **`Ctrl` + `O`** -> **`Enter`**, then press **`Ctrl` + `X`**.

---

## 🛠️ STEP 3: APPLY PERMANENT PATCH AGAINST SANDBOX CRASHES

### 1. Root Cause of Connection Drops
Modern Android versions (Android 14/15) prevent unrooted application environments from polling system MAC addresses due to strict privacy sandboxing. When a remote browser initializes a workspace link, the core VS Code loop spawns an automated hardware profile validation through native binaries inside the `@vscode/deviceid` module. Getting a null return payload forces an unhandled `Unsupported platform` crash loop, causing Android's kernel to invoke the Out-Of-Memory/Low-Memory manager to terminate (`Kill`) the process, abruptly ending your SSH or ADB terminal stream.

### 2. Override the Binary Check Using Pure JavaScript (Run Only Once)
To bypass this structural roadblock, we override the crashing binary file with a pure JavaScript object literal mapped to standard Node.js CommonJS logic. This forces the function hook to bypass hardware checks and instantly drop a static machine string layout:

```bash
cat << 'INNER_EOF' > /data/data/com.termux/files/usr/lib/code-server/lib/vscode/node_modules/@vscode/deviceid/dist/index.js
module.exports = {
    getDeviceId: async function() {
        return "android-s26-ultra-fixed-id";
    }
};
INNER_EOF
```

---

## 🚀 STEP 4: INSTANTIATING AND CODING FROM YOUR PC

### 1. Launch a Clean, Silent Instance on Android
Run the engine while redirecting unexpected error stack streams to prevent terminal log pollution:
```bash
code-server --auth none --disable-telemetry 2>/dev/null
```

### 2. Link Up via Your PC Browser
1. **Locate your Device Local IP:** Open a secondary working tab inside Termux and run `ifconfig`. Look down the `wlan0` interface section for your assigned wireless local IP address (e.g., `192.168.1.15`).
2. **Access via Browser:** Bring up any browser app on your desktop computer (Chrome, Edge, etc.) and type:
   👉 **`http://<YOUR_PHONE_IP>:8080`** *(e.g., `http://192.168.1.15:8080`)*
3. Your development connection will now stay alive indefinitely. You can build projects and save workspace modifications cleanly without sudden dropouts.

### 💡 Performance Pro-Tip: Preventing Android CPU Throttling
Before launching a deep coding workflow session, run this background utility command inside Termux to prevent Android from entering a power-saving deep sleep state when the handset screen lock engages:
```bash
termux-wake-lock
```
