# DIANA
`git@github.com:DianaAssistant/DIANA.git`

A voice/text controlled AI assistant that learns from its user usage of iOS/Android devices

## Scenarios

### Order food

### Buy airplane tickets

### Buy cheapest USB hub

### Create calendar event with given parameters

### Setup a new device using provided Wifi credentials

### Login into Google Accounts

## Usage

## Architecture

## Architecture/Usage

User :older_woman: runs an [app](#UnrealRDPClient) on device :robot: :apple: :iphone: :computer: :eyeglasses: . It connects to server :cloud: that runs [broker service](#PurposeRemoteDesktopSessionWebBroker). User :older_woman:  presses "start new session" button. App asks him about purpose of the session. After response, RDP session from user :older_woman: device to remote :robot: Android :iphone:  phone running [RDP server that shows camera feed and translates mouse/key commands](#AndroidCameraUSBGadgetRDPServer) to the USB connected to the :apple: :iphone: iOS device as USB peripheral.


All user :older_woman: actions are logged.   After session is ended, user :older_woman: is presented with choice of storing session as routine. User :older_woman: is offered with a choice of marking parameters in session description as a typed wildcards.

In future, if user :older_woman: starts a new session and sets its purpose to be similiar as purposes of sessions of the past, [app](#UnrealRDPClient)  :robot: :apple: :iphone: :computer: :eyeglasses: will give user :older_woman: an option to execute stored routine for user.


### Components

#### RemoteDesktopCaptureMLConverter

native format (capture=2 in RDP.ini file as per ReDemPtion readme) to ML (numpy readable csv containing OCRed ui elements coordinates and text in key frames + user actions clumped together, everything timeframed) conventer. Uses keyboard entered &&&&for session purpose notation@@@@

#### [UnrealRDPUILoggingOverlay]

an Unreal app that proxies RDP and on keystroke presents user with interface overlay containing some UWidgets including buttons, radio buttons, checkmarks and Text fields. each state change of UWidgets should be logged together with UnrealReDemption logging 

Built with: UnrealReDemPtion

#### [DianaUILoggingOverlay]

text field "objective" 
button wirh marker that makes cursor look like marker, then OCRs selected text and adds it onto clipboard stack
button remove text from stack and paste
button paste text from stack
keybindings (on joystick)


#### [UnrealRDPClient](https://github.com/DianaAssistant/UnrealRDPClient) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/UnrealRDPClient)
`git@github.com:DianaAssistant/UnrealRDPClient.git`


windows/iphone/iPad/android/hololens 2 RDP client

Built with: UnrealReDemPtion

#### UnrealReDemPtion
Unreal plugin that starts an RDP proxy, draws remote display on texture/actor , passes an Unreal camera view texture through, catches clicks and keystrokes with Unreal UI



#### [AndroidCameraUSBGadgetRDPServer](https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer) [ :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/AndroidCameraUSBGadgetRDPServer)
`git@github.com:DianaAssistant/AndroidCameraUSBGadgetRDPServer.git`


RPD server running on android presenting view from camera, transfering keypresses and mouse movements via tejado/android-usb-gadget just like tejado/Authorizer

### Deps



#### [redemption](https://github.com/DianaAssistant/redemption) [repo :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/redemption)
`git@github.com:DianaAssistant/redemption.git`


A GPL RDP proxy

#### [android-usb-gadget](https://github.com/DianaAssistant/android-usb-gadget) [repo :octocat:](x-github-client://openRepo/https://github.com/DianaAssistant/android-usb-gadget)
`git@github.com:DianaAssistant/android-usb-gadget.git`


Convert your Android phone to any USB device you like! USB Gadget Tool allows you to create and activate USB device roles, like a mouse or a keyboard.


# Hardware reqs

## Camera/Keysim unit

Android device, intersection of 

```

Google_Pixel 3 XL_Pixel 3 XL_Working
HUAWEI_SNE-LX1_SNE-LX1_Working
Xiaomi_MI 9_MI 9_Working
Xiaomi_Redmi Note 3_Redmi Note 3_Working
Xiaomi_POCO F1_POCO F1_Working;Jul 19, 2019
Google_Pixel 3a_Pixel 3a_Working;Jul 20, 2019
Google_Pixel 3_Pixel 3_Working;Jul 23, 2019
samsung_SM-T590_SM-T590_Working;Jul 23, 2019
Huawei_LYA-L29_HUAWEI Mate 20 Pro_Working;Aug 30, 2019
Samsung_SM-G965F_Galaxy S9+_Working;Sep 19, 2019
Huawei_ELE-L29_HUAWEI P30_Working;Nov 14, 2019
Sony_Xperia Z2 Tablet WiFi_Xperia Z2 Tablet WiFi_Working;Mar 29, 2020
Samsung_SM-A705MN_Galaxy A70_Working;Feb 9, 2020
Huawei_JSN-L23_Honor 8X_Working;Feb 10, 2020
Samsung_SM-N975F_Galaxy Note10+_Working;Mar 21, 2020
samsung_SM-G960F_Galaxy S9_Working;May 18, 2020
ZTE_Blade A7 2019-T_Blade A7 2019-T_Working;Jun 3, 2020
samsung_SM-G960N_Galaxy S9_Working;Jun 24, 2020
HUAWEI_JAT-LX1_JAT-LX1_Working;Aug 21, 2020
samsung_SM-M215F_SM-M215F_Working;Sep 24, 2020
samsung_SM-G398FN_SM-G398FN_Working;Oct 7, 2020
Xiaomi_Redmi Note 9 Pro_Redmi Note 9 Pro_Not Working;Oct 23, 2020
samsung_SM-T590_SM-T590_Working;Oct 26, 2020
HUAWEI_TAS-AN00_TAS-AN00_Working;Oct 30, 2020
HUAWEI_VCE-AL00_VCE-AL00_Working;Oct 31, 2020
Google_Pixel_Pixel_Working,Nov 1, 2020
Xiaomi_M2002J9G_M2002J9G_Working;Nov 3, 2020
Sony_XQ-AU51_XQ-AU51_Working;Nov 15, 2020
Xiaomi_M2006J10C_M2006J10C_Working;Nov 20, 2020
samsung_SM-M515F_SM-M515F_Working;Feb 2, 2021
```

and whichever of them can be rooted




























