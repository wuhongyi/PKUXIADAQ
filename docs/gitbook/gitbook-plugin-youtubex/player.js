require(["gitbook", "jquery"], function (gitbook, $) {
  window.onYouTubeIframeAPIReady = function () {
    $('.youtubex').each(function () {
      id = $(this).attr('id');
      var player = new YT.Player(id, {
        height: '100%',
        width: '100%',
        playerVars: {
          'controls': 1,
          'autoplay': 0,
          'modestbranding': 1,
          'rel': 0
        },
        videoId: id
      });

      $('.skip_' + id).click(function () {
        var time = $(this).data('time');
        player.seekTo(time, true);
      });
    });
  };

  gitbook.events.bind("page.change", window.onYouTubeIframeAPIReady);
});
