function dp(pixelDensity,x) {
	return (pixelDensity * 25.4 < 120) ? x : x*(pixelDensity * 25.4/160);
}

function baseColor() {
	return "#f26e35"
}

function primaryColor() {
    return "#000000"
}

function textColor() {
    return "#000000"
}

function textAltColor() {
	return "#a6aaa2"
}

function backgroundColor() {
	return "#e7e8e9"
}

function backgroundAltColor() {
	return "#ffefde"
}

