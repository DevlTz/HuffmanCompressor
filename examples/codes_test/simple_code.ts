//Implemente uma função que calcule o fatorial de um número.
function fatorial(n: number): number {
  if (n === 1) {
    // Essa é a condição de parada da recursão, conhecida como caso base.
    return 1;
  }
  return n * fatorial(n - 1); //Essa é a recursão, a função retorna o valor de n multiplicado pelo fatorial de (n - 1). Então ela chama a si mesma com um valor menor (n - 1).
}
console.log(fatorial(8));