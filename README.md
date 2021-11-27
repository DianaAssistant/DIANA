# VoiceTextControlledGUI
`git@github.com:DianaAssistant/VoiceTextControlledGUI.git`

A voice/text controlled AI assistant that learns from its user usage of iOS/Android devices

## Scenarios

### Order food

### Buy airplane tickets

### Buy cheapest USB hub

### Create calendar event with given parameters

### Setup a new device using provided Wifi credentials

### Login into Google Accounts

## Architecture

User :user: runs an [app](#UnrealRDPClient) on device :iphone: :computer: :glasses: . It connects to server :server: that runs [broker service](#PurposeRemoteDesktopSessionWebBroker). User presses "start new session" button. App asks him about purpose of the session. After response, RDP session from user device to remote Adroid phone running [RDP server that shows camera feed and translates mouse/key commands](#AndroidCameraUSBGadgetRDPServer) to the USB connected to the iOS device as USB peripheral.


### Components

#### RemoteDesktopCaptureMLConverter

native format (capture=2 in RDP.ini file as per ReDemPtion readme) to ML (numpy readable csv containing OCRed ui elements coordinates and text in key frames + user actions clumped together, everything timeframed) conventer. Uses keyboard entered &&&&for session purpose notation@@@@

#### [PurposeRemoteDesktopSessionWebBroker](https://github.com/DianaAssistant/PurposeRemoteDesktopSessionWebBroker) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/PurposeRemoteDesktopSessionWebBroker)
`git@github.com:DianaAssistant/PurposeRemoteDesktopSessionWebBroker.git`


Remote Desktop Session Web Broker that asks user for purpose of session and cleans up after the session. saves preserves state of the physical device.

#### [UnrealTextureOCR](https://github.com/DianaAssistant/UnrealTextureOCR) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/UnrealTextureOCR)
`git@github.com:DianaAssistant/UnrealTextureOCR.git`


UnrealTextureOCR: windows, hololens 2, android

#### [UnrealRDPClient](https://github.com/DianaAssistant/UnrealRDPClient) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/UnrealRDPClient)
`git@github.com:DianaAssistant/UnrealRDPClient.git`


windows/iphone/iPad/android/hololens 2 RDP client with support of public servers though public server broker

#### AndroidCameraUSBGadgetRDPServer](https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer)
`git@github.com:DianaAssistant/AndroidCameraUSBGadgetRDPServer.git`


RPD server running on android presenting view from camera, transfering keypresses and mouse movements via tejado/android-usb-gadget just like tejado/Authorizer

### Deps

#### [AndroidCameraUSBGadgetRDPServer](https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer)
`git@github.com:DianaAssistant/AndroidCameraUSBGadgetRDPServer.git`


RPD server running on android presenting view from camera, transfering keypresses and mouse movements via tejado/android-usb-gadget just like tejado/Authorizer

#### [redemption](https://github.com/DianaAssistant/redemption) [repo :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/redemption)
`git@github.com:DianaAssistant/redemption.git`


A GPL RDP proxy

#### [android-usb-gadget](https://github.com/DianaAssistant/android-usb-gadget) [repo :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/android-usb-gadget)
`git@github.com:DianaAssistant/android-usb-gadget.git`


Convert your Android phone to any USB device you like! USB Gadget Tool allows you to create and activate USB device roles, like a mouse or a keyboard.

