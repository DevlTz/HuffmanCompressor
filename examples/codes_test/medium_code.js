const senhaExibida = document.getElementById("senha");
const feedbackExibido = document.getElementById("feedback");
const botaoTamanhoPersonalizavel = document.getElementById("comprimento");
const buttonTamanhos = document.getElementById("botao-tamanho");
const opcoesTamanho = document.querySelector(".tamanhos");
const tamanhosFixos = document.querySelectorAll(
  ".tamanhos input[type='checkbox']"
);
let tamanhoFixoEscolhido = null;

//Feedback para o usuário
const mostrarFeedback = (mensagem, tipo) => {
  feedbackExibido.textContent = mensagem;
  feedbackExibido.style.color = tipo === "erro" ? "red" : "green";
  feedbackExibido.style.fontWeight = "bold";
};
//Geração das senhas
const gerarSenha = (comprimento, opcoes) => {
  if (comprimento <= 0 || comprimento == NaN) {
    mostrarFeedback(
      "Inválido! Para genhar uma senha, é necessário inserir um número maior que 0",
      "erro"
    );
    return null;
  }

  let caracteres = "";
  if (opcoes.maiusculas) {
    caracteres += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  }
  if (opcoes.minusculas) {
    caracteres += "abcdefghijklmnopqrstuvwxyz";
  }
  if (opcoes.numeros) {
    caracteres += "0123456789";
  }
  if (opcoes.especiais) {
    caracteres += "!@#$%^&*()_+[]{}|;:,.<>?";
  }
  if (caracteres.length == 0) {
    mostrarFeedback(
      "Inválido! Para genhar uma senha, é necessário marcar pelo menos uma das opções!",
      "erro"
    );
    return null;
  }

  let senha = "";
  for (let i = 0; i < comprimento; i++) {
    //Math.random() gera um número entre 0 e 1, sem incluir 1, e, quando multiplicado pelo comprimento do array, é dimensionado para o intervalo do array. Math.floor()Em seguida, é usado para arredondar para baixo, para o número inteiro mais próximo, garantindo que o índice seja válido.
    let indice = Math.floor(Math.random() * caracteres.length);
    senha += caracteres[indice];
  }
  mostrarFeedback("Senha gerada com sucesso!", "sucesso");
  return senha;
};

//Caso o usuário opte por tamanhos FIXOS

buttonTamanhos.addEventListener("click", () => {
  opcoesTamanho.classList.remove("tamanhos");
  buttonTamanhos.classList.add("tamanhos");
  botaoTamanhoPersonalizavel.classList.add("tamanhos");
  mostrarFeedback("Escolha UMA das opções de tamanho!", "sucesso");
});

//Atualizar o valor do tamanhofixo de acordo com marcação do checkbox
tamanhosFixos.forEach((checkboxSelecionado) => {
  checkboxSelecionado.addEventListener("change", () => {
    if (checkboxSelecionado.checked) {
      // Desmarcar os outros
      tamanhosFixos.forEach((outroCheckbox) => {
        if (outroCheckbox !== checkboxSelecionado)
          outroCheckbox.checked = false;
      });
      tamanhoFixoEscolhido = checkboxSelecionado.getAttribute("value");
    } else {
      tamanhoFixoEscolhido = null;
    }
  });
});

document.getElementById("gerar").addEventListener("click", function () {
  const opcoes = {
    maiusculas: document.getElementById("maiusculas").checked,
    minusculas: document.getElementById("minusculas").checked,
    numeros: document.getElementById("numeros").checked,
    especiais: document.getElementById("especiais").checked,
  };
  const comprimento =
    tamanhoFixoEscolhido || parseInt(botaoTamanhoPersonalizavel.value);

  const senha = gerarSenha(comprimento, opcoes);

  if (senha) {
    senhaExibida.textContent = senha;
    document.getElementById("copiar").style.display = "inline";
  }
});

document.getElementById("copiar").addEventListener("click", function () {
  let senha = document.getElementById("senha").textContent;
  if (senha.length < 1) {
    mostrarFeedback("Não é possível copiar uma senha inválida!", "erro");
    return null;
  }
  navigator.clipboard.writeText(senha).then(function () {
    mostrarFeedback("Senha copiada para a área de transferência!", "sucesso");
  });
});
