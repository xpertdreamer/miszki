## Dependencies

If you wanna build on your local machine, you should have the following instruments:

  - **gcc** - compiler used to build some laboratories
  - **make** - build automation tool 
  - **go** - compiler used to build some laboratories
  - **doc2go** - tool that generates static HTML documentation from Go code

Otherwise, if you have **Podman** on your machine, just run the container with the virtual environment inside:

``` shell
    podman build . -t mis:latest
    podman run --rm -v .:/app env:latest [target to build] <-d>
```

The build script runs automatically inside the container to build the specified target. The list of all targets can be found in the [make](#make-commands) section.

You can provide the `-d` flag at the end of the Podman run command to generate Doxygen/doc2go documentation.

## Make commands

Build one target from list:

``` shell
    make -B <name from the following list>
```

Remove build directory:

``` shell
    make clean
```

Run tests for target (supports *Go*-projects only):

``` shell
    make test [PKG=<target>]
```

List of targets:

  - lab1 - Laboratory Work №1
  - lab2 - Laboratory Work №2

## Documentation

While I'm not sure how to create static documentation using Go tools, please use dynamic documentation:

``` shell
cd [target] && godoc -http localhost:6060
```

Then open in your browser following link: ``http://localhost:6060/pkg/mizski/``

> [!NOTE]
> You can also provide the ``DOCS=1`` flag after the make target, and the documentation will be generated automatically into the `docs/html` directory.

## Laboratory Work 1
### References:
  - <https://en.wikipedia.org/wiki/Substitution_cipher>
  - [lecture](https://github.com/xpertdreamer/miszki/blob/ca7d8455ca3a5188ff0776b08f3913d2265f01e8/docs/manuals/%D0%9B%D0%B5%D0%BA%D1%86%D0%B8%D0%B8/%D0%9B%D0%B5%D0%BA%D1%86%D0%B8%D1%8F%20%E2%84%961.pptx)
  - <https://www.opennet.ru/man.shtml?topic=getopt&category=3&russian=0>
  - <https://stackoverflow.com/questions/8032080/how-to-convert-char-to-wchar-t>
  - <https://stackoverflow.com/questions/16931244/checking-if-output-of-a-command-contains-a-certain-string-in-a-shell-script>
  - <https://stackoverflow.com/questions/40082346/how-to-check-if-a-file-exists-in-a-shell-script>
  - <https://stackoverflow.com/questions/3349105/how-can-i-set-the-current-working-directory-to-the-directory-of-the-script-in-ba>
  - <https://en.wikipedia.org/wiki/Cyrillic_script_in_Unicode>
  - <http://blog.kislenko.net/show.php?id=2045>
  - <https://stackoverflow.com/questions/64518663/reading-from-file-and-store-it-to-string-with-unknown-length-in-c>
  - <https://www.opennet.ru/man.shtml?topic=wcstok&category=3&russian=0>
  - <https://man7.org/linux/man-pages/man3/swscanf.3p.html>
  - <https://stackoverflow.com/questions/9344477/how-to-print-wchar-t-array-to-file-in-64-bit-windows>

## Laboratory Work 2
### References
  - <https://pkg.go.dev/log>
  - <https://habr.com/ru/companies/otus/articles/782812/>
  - <https://stackoverflow.com/questions/24790175/when-does-the-init-function-run>
  - <https://habr.com/ru/companies/otus/articles/833702/>
  - <https://blog.jetbrains.com/go/2022/11/22/comprehensive-guide-to-testing-in-go/>
  - <https://stackoverflow.com/questions/24489384/how-to-print-the-values-of-slices>
  - <https://go.dev/doc/tutorial/handle-errors>
  - <https://github.com/TheAlgorithms/Go/blob/master/math/prime/primecheck.go>
  - <https://stackoverflow.com/questions/15311969/checking-the-equality-of-two-slices>
  - <https://pkg.go.dev/fmt>
  - <https://stackoverflow.com/questions/10485743/contains-method-for-a-slice>
  - <https://cp-algorithms.com/algebra/sieve-of-eratosthenes.html>
  - <https://en.wikipedia.org/wiki/Perfect_number>
  - <https://leetcode.com/problems/perfect-number/solutions/127529/perfect-number/>
  - <https://stackoverflow.com/questions/6566835/algorithm-to-check-if-a-number-if-a-perfect-number>
  - <https://pkg.go.dev/math/big>
  - <https://en.wikipedia.org/wiki/List_of_Mersenne_primes_and_perfect_numbers>
  - <https://superuser.com/questions/296643/how-to-get-a-perfect-local-copy-of-a-web-page>
  - <https://github.com/pborman/getopt>
  - <https://stackoverflow.com/questions/46783352/string-to-big-int-in-go>
  - <https://pkg.go.dev/github.com/pborman/getopt#section-readme>
  
