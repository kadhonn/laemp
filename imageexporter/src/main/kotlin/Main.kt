package org.example

import java.io.FileInputStream
import java.util.*
import javax.imageio.ImageIO

const val DIMENSION = 8

fun main() {
//
//    val bytes = listOf(0,0,0,0,0,0,112,14,-8,31,-4,63,-4,63,-8,31,-8,31,-16,15,-32,7,-64,3,-128,1,0,0,0,0,0,0,)
//
//    for (y in 0 until 16) {
//        for (x in 0 until 16) {
//            val n = y * 16 + x
//            val active = bytes[n / 8].and(1.shl(n % 8)) != 0
//            print(if (active) "#" else " ")
//        }
//        println()
//    }

    val bufferedImage = ImageIO.read(FileInputStream("C:\\Users\\ablei\\Downloads\\bubble_8.png"))
    if (bufferedImage.height != DIMENSION || bufferedImage.width != DIMENSION) {
        println("buffered image has wrong dimensions ${bufferedImage.width}x${bufferedImage.height}")
        return
    }

    val bitSet = BitSet()

    for (i in 0 until (DIMENSION * DIMENSION)) {
        val x = i % DIMENSION
        val y = i / DIMENSION
        if (bufferedImage.getRGB(x, y) != 0) {
            bitSet.set(i)
        }
    }

    val byteArray = bitSet.toByteArray()
    for (i in 0 until (DIMENSION * DIMENSION / 8)) {
        if (byteArray.size > i) {
            print(byteArray[i])
        } else {
            print(0)
        }
        print(",")
    }

}