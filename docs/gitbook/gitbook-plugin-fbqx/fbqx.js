require(["gitbook", "jquery"], function (gitbook, $) {

  var init = function () {

    if (!$('.toolbar-delete').length)
      gitbook.toolbar.createButton({
        position: 'left',
        icon: 'fa fa-history',
        className: 'toolbar-delete',
        onClick: function () {
          $('.gitQuestion').each(function () {
            delete localStorage[$(this).data('id')];
          });
          $(this).prepend('<p class="historyMessage">History cleared. Refreshing ...</p>');
          $('.historyMessage').hide().fadeIn('slow').delay(2000).fadeOut('slow').delay(1).queue(function () {
            $(this).remove();
            location.reload();
          });
        }
      });

    $('.fbqx').each(function () {
      var $fbqBox = $(this).children('.FBQbox');
      var qid = $fbqBox.data('id');
      var answer = $fbqBox.data('answer');

      $fbqBox.find('.FBQmessage').hide();

      if (localStorage[qid]) {
        $fbqBox.find('input').each(function (i) {
          $(this).val(answer[i]).prop('readonly', true);
        });
        $fbqBox.find('button').addClass('disabled');
        $fbqBox.find('.FBQmessage').show();
      }

      $fbqBox.find('.FBQsubmit').click(function () {
        var allCorrect = true;
        var ans = [];

        $fbqBox.find('.ans').each(function (i) {
          var input = $(this).children('input').val();

          if (answer[i] == input)
            $(this).children('input').addClass('correct').removeClass('wrong').prop('readonly', true);
          else {
            $(this).children('input').addClass('wrong');
            allCorrect = false;
          }
        });

        if (allCorrect) {
          localStorage[qid] = true;
          $fbqBox.find('button').addClass('disabled');
          $fbqBox.find('.FBQmessage').show('slow');
        }
      });

      $fbqBox.find('.ans > input').click(function () {
        $(this).removeClass('wrong');
      });

      $fbqBox.find('.ans > input').keypress(function () {
        $(this).removeClass('wrong');
      });

    });
  };

  gitbook.events.bind("page.change", init);
});
