<?php $i=0; foreach($data["team"] as $reseller) { ?>

  <div class="row project-row <?= ($i%2 != 0) ? "even" : "odd" ?>">
	<div class="sixteen columns">
	  <?= drupal_render($reseller); ?>
	</div>
  </div>

<?php $i++; } ?>