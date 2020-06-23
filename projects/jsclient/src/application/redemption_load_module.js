const redemptionLoadModule = function(Module, window)
{
    const HEAPU8 = Module.HEAPU8;
    const HEAP16 = Module.HEAP16;

    const identity = function(cb, thisp) {
        // TODO return cb; ?
        return function(...args) {
            return cb.call(thisp, ...args);
        };
    };

    const noop = function(){};

    // { funcname: [wrapCreator, defaultFunction], ... }
    const wrappersGd = {
        setBmpCacheEntries: identity,
        setBmpCacheIndex: identity,
        drawMemBlt: identity,
        drawMem3Blt: identity,

        drawImage: identity,
        drawRect: identity,
        drawScrBlt: identity,
        drawLineTo: identity,
        drawDestBlt: identity,
        drawPolyline: identity,
        drawPolygoneSC: identity,
        drawPolygoneCB: identity,
        drawEllipseSC: identity,
        drawEllipseCB: identity,
        drawPatBlt: identity,
        drawFrameMarker: identity,

        setPointer: identity,
        newPointer: identity,
        cachedPointer: identity,

        resizeCanvas: identity,
        updatePointerPosition: identity,
    };

    const wrapEvents = function(wrappedEvents, wrappers, events, defaultCb) {
        for (const eventName in wrappers) {
            const wrapCb = wrappers[eventName];
            const cb = events[eventName];
            wrappedEvents[eventName] = cb ? wrapCb(cb, events) : defaultCb;
        }
    };


    class RDPClient {
        constructor(socket, graphics, config) {
            this.native = new Module.RdpClient(graphics, config);
            this.socket = socket;
            this._channels = [];
        }

        close() {
            this.socket.close();
        }

        start() {
            this.native.sendFirstPacket();
            this.sendBufferedData();
        }

        delete() {
            for (const channel of this._channels) {
                channel.delete();
            }
            this.native.delete();
        }

        getCallback() {
            return this.native.getCallbackAsVoidPtr();
        }

        addChannel(channel) {
            this._channels.push(channel);
            this.native.addChannelReceiver(channel.getChannelReceiver());
        }

        sendUnicode(unicode, flags) {
            this.native.sendUnicode(unicode, flags);
            this.sendBufferedData();
        }

        sendScancode(key, flags) {
            this.native.sendScancode(key, flags);
            this.sendBufferedData();
        }

        sendMouseEvent(flag, x, y) {
            this.native.sendMouseEvent(flag, x, y);
            this.sendBufferedData();
        }

        sendBufferedData() {
            const out = this.native.getOutputData();
            if (out.length) {
                this.socket.send(out);

                // let text = "";
                // console.log('Received Binary Message of ' + out.length + ' bytes');
                // for (let byte of out) {
                //     text += ":" + (byte+0x10000).toString(16).substr(-2);
                // }
                // this.counter = (this.counter || 0) + 1
                // console.log(this.counter, "Send: " + text);

                this.native.resetOutputData();
            }
        }

        addReceivingData(data) {
            this.native.pushInputData(data);
        }
    };

    const wrappersPlayer = {
        setPointerPosition: identity,
        setTime: identity,
    };

    const resultFuncs = {
        RDPClient: RDPClient,
        newRDPSocket: function(url) {
            const socket = new WebSocket(url, "RDP");
            socket.binaryType = 'arraybuffer';
            return socket;
        },
        newWrmPlayer: function(events) {
            const playerEvents = {};
            wrapEvents(playerEvents, wrappersGd, events, noop);
            wrapEvents(playerEvents, wrappersPlayer, events, noop);
            return new Module.WrmPlayer(playerEvents);
        }
    };

    const addChannelClass = (ChannelClassName, wrappers) => {
        resultFuncs['new' + ChannelClassName] = function(cb, events, ...channelArgs) {
            wrapEvents(events, wrappers, events, noop);
            const chann = new Module[ChannelClassName](cb, events, ...channelArgs);
            const setChann = events['setEmcChannel'];
            if (setChann) {
                setChann.call(events, chann);
            }
            return chann;
        };
    };

    addChannelClass('ClipboardChannel', {
        setGeneralCapability: identity,
        formatListStart: identity,
        formatListFormat: identity,
        formatListStop: identity,
        formatDataResponse: identity,
        formatDataResponseFileStart: identity,
        formatDataResponseFile: identity,
        formatDataResponseFileStop: identity,
        formatDataRequest: identity,
        fileContentsRequest: identity,
        fileContentsResponse: identity,
        receiveResponseFail: identity,
        lock: identity,
        unlock: identity,
        free: identity,
    });

    addChannelClass('CustomChannel', {
        receiveData: identity,
        free: identity,
    });

    return resultFuncs;
};
