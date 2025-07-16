## FraudDetection v1 - Setup Guide

### Prerequisites

-   [Node.js](https://nodejs.org/) (v16.x or newer recommended)
-   [npm](https://www.npmjs.com/)
-   [Truffle](https://trufflesuite.com/truffle/) (`npm install -g truffle`)
-   [Ganache](https://trufflesuite.com/ganache/) (Desktop or CLI)
-   [MetaMask](https://metamask.io/) browser extension (Chrome/Brave/Edge)

---

### Installation

1. **Install dependencies:**

    ```sh
    npm install
    ```

2. **Compile smart contracts:**

    ```sh
    truffle compile
    ```

3. **Start Ganache (local blockchain):**

    - Open Ganache and create a new workspace.
    - Add your project's `truffle-config.js` to the workspace.
    - Set the server port to `7545` (to match `truffle-config.js`).

4. **Configure MetaMask:**

    - Open MetaMask in your browser.
    - Add a new network:
        - **Network Name:** Local Ganache
        - **RPC URL:** `http://127.0.0.1:7545`
        - **Chain ID:** `1337`
        - **Currency Symbol:** `ETH`
        - **Network ID:** `5777` (from Ganache settings)
    - Import an account using the private key from Ganache.

5. **Deploy contracts:**

    ```sh
    truffle migrate --reset
    ```

6. **Start the development server:**

    ```sh
    npm run dev
    ```

    - This should open `index.html` in your default browser.

7. **Connect MetaMask:**

    - Log in to MetaMask.
    - Click 'Connect Wallet' on the homepage to link your account to the dApp.

8. **Interact with the website.**

---

### Troubleshooting

-   If contracts are not detected:
    -   Delete the contract in Ganache, remove it, then re-add and redeploy.
    -   Ensure MetaMask is connected to the correct local network.

---

### Notes

-   For production, consider using [Hardhat](https://hardhat.org/) for improved developer experience.
-   Always keep your private keys secure.
-   For more details, see [Truffle Documentation](https://trufflesuite.com/docs/truffle/).
