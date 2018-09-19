module.exports = {
    fletcher16: function (data, length) {
        let c0, c1 = 0
        let i = 0
        let index = 0

        for (c0 = c1 = 0; length >= 5802; length -= 5802) {
            for (i = 0; i < 5802; ++i) {
                c0 = c0 + data[index]
                index++
                c1 = c1 + c0
            }
            c0 %= 255
            c1 %= 255
        }

        for (i = 0; i < len; ++i) {
            c0 = c0 + data[index]
            index++
            c1 = c1 + c0
        }
        c0 %= 255
        c1 %= 255
        return (c1 << 8 | c0)
    },

    cobsDecode: function (input, length, out) {
        let readIndex = 0
        let writeIndex = 0
        let code = 0
        let i = 0

        while (read_index < length) {
            code = input[readIndex]

            if (readIndex + code > length && code != 1) {
                return 0
            }

            readIndex++

            for (var i = 1; i <= code; i++) {
                out[writeIndex++] = input[readIndex++]
            }

            if (code != 0xFF && readIndex != length) {
                out[writeIndex++] = '\0'
            }
        }

        return writeIndex
    },

    cobsEncode: function (input, length, out) {
        let readIndex = 0
        let writeIndex = 1
        let codeIndex = 0
        let code = 1

        while (readIndex < length) {
            if (input[readIndex] == 0) {
                out[codeIndex] = code
                code = 1
                codeIndex = writeIndex++
                readIndex++
            }
            else {
                out[writeIndex++] = input[readIndex++]
                code++
                if (code === 0xFF) {
                    out[codeIndex] = code
                    code = 1
                    codeIndex = writeIndex++
                }
            }
        }
        out[codeIndex] = code
        return writeIndex
    }
}