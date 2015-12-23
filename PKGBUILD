pkgname=ubfc
pkgver=1.0
pkgrel=1
pkgdesc="Useless Brainfuck Compiler: very simple compiler from Brainfuck to C language"
arch=(i686 x86_64)
url="https://apathism.net/"
license=('GPLv3')
depends=('gcc' 'bash')
source=('ubfc'
	'ubfc_gc.cpp'
	'ubfc_template.c')
md5sums=('9e8241b03a68007f328a3f81a8939996'
         '9a460ac7ee72b066a0a44253cb70363e'
         'f4c4304f64b727d39ac4e318422ffc88')

build() {
        cd "${srcdir}"
	g++ -O3 -o ubfc_gc ubfc_gc.cpp
}

package() {
        cd "${srcdir}"
	
	install -DTm755 ubfc "${pkgdir}/usr/bin/ubfc"
	install -DTm755 ubfc_gc "${pkgdir}/usr/bin/ubfc_gc"
	install -DTm644 ubfc_template.c "${pkgdir}/usr/share/ubfc/template.c"
	echo "$pkgver" > "${pkgdir}/usr/share/ubfc/version"
}
