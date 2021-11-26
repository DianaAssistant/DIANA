# VoiceTextControlledGUI

### Components

#### MLRemoteComputingSessionWebLogger 
A web server that provides an endpoint for UnrealMLRemoteComputingSessionWebLogger for logging an OCRed remote session over HTTP and saves it in Python readable ML friendly format, together with correct timestamps of session recorded by redemption RDP proxy server

##### UnrealMLRemoteComputingSessionWebLogger


#### PurposeRemoteDesktopSessionWebBroker 
Remote Desktop Session Web Broker that asks user for purpose of session and cleans up after the session. saves preserves state of the physical device.

#### UnrealTextureOCR 
UnrealTextureOCR: windows, hololens 2, android


### Deps
#### UnrealRDPClient 
windows/iphone/iPad/android/hololens 2 RDP client with support of public servers though public server broker

#### AndroidCameraUSBGadgetRDPServer 
RPD server running on android presenting view from camera, transfering keypresses and mouse movements via tejado/android-usb-gadget just like tejado/Authorizer

#### redemption 
A GPL RDP proxy

#### android-usb-gadget 
Convert your Android phone to any USB device you like! USB Gadget Tool allows you to create and activate USB device roles, like a mouse or a keyboard.

