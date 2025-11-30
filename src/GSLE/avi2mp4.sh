#!/bin/bash
ffmpeg -i render-chinaVase.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-chinaVase.mp4
ffmpeg -i render-woodenSculpture.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-woodenSculpture.mp4
ffmpeg -i render-teddyBear.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-teddyBear.mp4
ffmpeg -i render-monkeyStatue.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-monkeyStatue.mp4
ffmpeg -i render-birdHouse.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-birdHouse.mp4
ffmpeg -i render-knightStatue.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-knightStatue.mp4
ffmpeg -i render-penguin.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-penguin.mp4
ffmpeg -i render-turtle.avi -c:v libx264 -crf 18 -preset slow -c:a aac render-turtle.mp4